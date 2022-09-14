#include "..\Headers\Cell.h"
#include "..\Headers\PipeLine.h"

CCell::CCell(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
{
	Safe_AddRef(m_pDevice);
}

HRESULT CCell::Ready_Cell(_vec3 vPointA, _vec3 vPointB, _vec3 vPointC, _uint iIndex)
{
	m_vShareVector[POINT_A] = vPointA;
	m_vShareVector[POINT_B] = vPointB;
	m_vShareVector[POINT_C] = vPointC;

	m_vShareLine[LINE_AB] = m_vShareVector[POINT_B] - m_vShareVector[POINT_A];
	m_vShareLine[LINE_BC] = m_vShareVector[POINT_C] - m_vShareVector[POINT_B];
	m_vShareLine[LINE_CA] = m_vShareVector[POINT_A] - m_vShareVector[POINT_C];

	m_vNormal[LINE_AB] = _vec3(m_vShareLine[LINE_AB].z * -1.f, 0.0f, m_vShareLine[LINE_AB].x);
	m_vNormal[LINE_BC] = _vec3(m_vShareLine[LINE_BC].z * -1.f, 0.0f, m_vShareLine[LINE_BC].x);
	m_vNormal[LINE_CA] = _vec3(m_vShareLine[LINE_CA].z * -1.f, 0.0f, m_vShareLine[LINE_CA].x);

	for (size_t i = 0; i < LINE_END; i++)
	{
		D3DXVec3Normalize(&m_vNormal[i], &m_vNormal[i]);
	}

	m_iIndex = iIndex;

#ifdef _DEBUG
	if (FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
		return E_FAIL;
#endif // DEBUG


	ZeroMemory(m_pNeighbor, sizeof(CCell*) * NEIGHBOR_END);


	return S_OK;
}

_bool CCell::Compare_Point(_vec3 vSourPoint, _vec3 vDestPoint)
{
	if (vSourPoint == m_vShareVector[POINT_A])
	{
		if (vDestPoint == m_vShareVector[POINT_B])
			return true;

		if (vDestPoint == m_vShareVector[POINT_C])
			return true;
	}

	if (vSourPoint == m_vShareVector[POINT_B])
	{
		if (vDestPoint == m_vShareVector[POINT_C])
			return true;

		if (vDestPoint == m_vShareVector[POINT_A])
			return true;
	}

	if (vSourPoint == m_vShareVector[POINT_C])
	{
		if (vDestPoint == m_vShareVector[POINT_A])
			return true;

		if (vDestPoint == m_vShareVector[POINT_B])
			return true;
	}

	return false;
}

_bool CCell::Check_Move(const _vec3 * pResultPos, CCell ** ppNeighborCell)
{
	LINE eLineId;

	if (!Is_In(pResultPos,&eLineId))
	{
		if(nullptr == m_pNeighbor[eLineId])
		return false;

		else
		{
			if (m_pNeighbor[eLineId]->Check_Move(pResultPos, ppNeighborCell))
			{
				*ppNeighborCell = m_pNeighbor[eLineId];
				return false;
			}

			else
				return false;
		}

	}
	else 
		return true;
}

_bool CCell::Is_In(const _vec3 * pResultPos, LINE * pLineId)
{
	for (int i = 0; i < LINE_END; i++)
	{
		_vec3 vTemp = *pResultPos - m_vShareVector[i];

		if (0 < D3DXVec3Dot(&m_vNormal[i], D3DXVec3Normalize(&vTemp, &vTemp)))
		{
			*pLineId = LINE(i);

			return false;
		}
	}
	return true;
}

#ifdef _DEBUG
HRESULT CCell::Render_Cell()
{

	if (nullptr == m_pLine)
		return E_FAIL;

	CPipeLine*	pPipeLine = CPipeLine::GetInstance();
	if (nullptr == pPipeLine)
		return E_FAIL;

	Safe_AddRef(pPipeLine);

	_vec3		vPoint[4] = {
		{ m_vShareVector[POINT_A] },
		{ m_vShareVector[POINT_B] },
		{ m_vShareVector[POINT_C] },
		{ m_vShareVector[POINT_A] },
	};


	for (size_t i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &pPipeLine->Get_Transform(D3DTS_VIEW));
		if (0 > vPoint[i].z)
			vPoint[i].z = 0.f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &pPipeLine->Get_Transform(D3DTS_PROJECTION));

	}

	_matrix		Identity;

	m_pLine->SetWidth(2.0f);

	m_pLine->Begin();

	m_pLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&Identity), D3DXCOLOR(1.f, 10.f, 1.f, 1.f));

	m_pLine->End();

	Safe_Release(pPipeLine);

	return S_OK;
}
#endif

CCell * CCell::Create(LPDIRECT3DDEVICE9 pDevice, _vec3 vecA, _vec3 vecB, _vec3 vecC, _uint iIndex)
{
	CCell* pInstance = new CCell(pDevice);

	if (FAILED(pInstance->Ready_Cell(vecA, vecB, vecC, iIndex)))
	{
		MSG_BOX("Failed while Creating CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
#ifdef _DEBUG
	Safe_Release(m_pLine);
#endif
	Safe_Release(m_pDevice);
}
