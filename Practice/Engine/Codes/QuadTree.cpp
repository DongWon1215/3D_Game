#include "..\Headers\QuadTree.h"
#include "..\Headers\Frustum.h"


CQuadTree::CQuadTree(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pPipeLine(CPipeLine::GetInstance())
{
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pGraphic_Device);

	ZeroMemory(m_pNeighbor, sizeof(CQuadTree*) * NEIGHBOR_END);
}

HRESULT CQuadTree::Ready_QuadTree(_vec3* pPosition, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	m_iCornerIndex[CORNER_LT] = iNumVerticesX * iNumVerticesZ - iNumVerticesX;
	m_iCornerIndex[CORNER_RT] = iNumVerticesX * iNumVerticesZ - 1;
	m_iCornerIndex[CORNER_RB] = iNumVerticesX - 1;
	m_iCornerIndex[CORNER_LB] = 0;

	m_iCenterIndex = (m_iCornerIndex[CORNER_LT] + m_iCornerIndex[CORNER_RB]) >> 1;

	m_fRadius = D3DXVec3Length(&(pPosition[m_iCornerIndex[CORNER_RT]] - pPosition[m_iCenterIndex]));

	Make_Child(pPosition);

	Make_Neighbor();

	return NOERROR;
}

HRESULT CQuadTree::Ready_QuadTree(_vec3 * pPosition, _uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
	m_iCornerIndex[CORNER_LT] = iLT;
	m_iCornerIndex[CORNER_RT] = iRT;
	m_iCornerIndex[CORNER_RB] = iRB;
	m_iCornerIndex[CORNER_LB] = iLB;

	if (1 == m_iCornerIndex[CORNER_RT] - m_iCornerIndex[CORNER_LT])
		return NOERROR;

	m_iCenterIndex = (m_iCornerIndex[CORNER_LT] + m_iCornerIndex[CORNER_RB]) >> 1;

	m_fRadius = D3DXVec3Length(&(pPosition[m_iCornerIndex[CORNER_RT]] - pPosition[m_iCenterIndex]));

	Make_Child(pPosition);

	return NOERROR;
}

HRESULT CQuadTree::Culling_ToQuadTree(CFrustum* pFrustum, _vec3* pPosition, INDEX32* pIndices, _uint* pNumPolygons)
{
	// if (nullptr == m_pChild[CHILD_LT])
	if (true == LevelofDetail(pPosition))
	{
		_bool		isDraw[NEIGHBOR_END] = { true, true, true, true };

		for (size_t i = 0; i < NEIGHBOR_END; ++i)
		{
			if (nullptr != m_pNeighbor[i])
				isDraw[i] = m_pNeighbor[i]->LevelofDetail(pPosition);
		}

		_bool		isIn[CORNER_END] = { 0 };


		for (size_t i = 0; i < CORNER_END; i++)
			isIn[i] = pFrustum->is_InFrustum(pPosition[m_iCornerIndex[i]], 0.0f);

		if (true == isDraw[NEIGHBOR_LEFT] &&
			true == isDraw[NEIGHBOR_TOP] &&
			true == isDraw[NEIGHBOR_RIGHT] &&
			true == isDraw[NEIGHBOR_BOTTOM])
		{
			if (true == isIn[CORNER_LT] ||
				true == isIn[CORNER_RT] ||
				true == isIn[CORNER_RB])
			{
				pIndices[*pNumPolygons]._1 = m_iCornerIndex[CORNER_LT];
				pIndices[*pNumPolygons]._2 = m_iCornerIndex[CORNER_RT];
				pIndices[*pNumPolygons]._3 = m_iCornerIndex[CORNER_RB];
				++(*pNumPolygons);
			}

			if (true == isIn[CORNER_LT] ||
				true == isIn[CORNER_RB] ||
				true == isIn[CORNER_LB])
			{
				pIndices[*pNumPolygons]._1 = m_iCornerIndex[CORNER_LT];
				pIndices[*pNumPolygons]._2 = m_iCornerIndex[CORNER_RB];
				pIndices[*pNumPolygons]._3 = m_iCornerIndex[CORNER_LB];
				++(*pNumPolygons);
			}

			return NOERROR;
		}

		_uint		iLC, iTC, iRC, iBC;

		iLC = (m_iCornerIndex[CORNER_LT] + m_iCornerIndex[CORNER_LB]) >> 1;
		iTC = (m_iCornerIndex[CORNER_LT] + m_iCornerIndex[CORNER_RT]) >> 1;
		iRC = (m_iCornerIndex[CORNER_RT] + m_iCornerIndex[CORNER_RB]) >> 1;
		iBC = (m_iCornerIndex[CORNER_LB] + m_iCornerIndex[CORNER_RB]) >> 1;


		if (true == isIn[CORNER_LT] ||
			true == isIn[CORNER_RB] ||
			true == isIn[CORNER_LB])
		{
			if (false == isDraw[NEIGHBOR_LEFT])
			{
				pIndices[*pNumPolygons]._1 = m_iCornerIndex[CORNER_LT];
				pIndices[*pNumPolygons]._2 = m_iCenterIndex;
				pIndices[*pNumPolygons]._3 = iLC;
				++(*pNumPolygons);

				pIndices[*pNumPolygons]._1 = iLC;
				pIndices[*pNumPolygons]._2 = m_iCenterIndex;
				pIndices[*pNumPolygons]._3 = m_iCornerIndex[CORNER_LB];
				++(*pNumPolygons);
			}

			else
			{
				pIndices[*pNumPolygons]._1 = m_iCornerIndex[CORNER_LT];
				pIndices[*pNumPolygons]._2 = m_iCenterIndex;
				pIndices[*pNumPolygons]._3 = m_iCornerIndex[CORNER_LB];
				++(*pNumPolygons);
			}

			if (false == isDraw[NEIGHBOR_BOTTOM])
			{
				pIndices[*pNumPolygons]._1 = m_iCornerIndex[CORNER_LB];
				pIndices[*pNumPolygons]._2 = m_iCenterIndex;
				pIndices[*pNumPolygons]._3 = iBC;
				++(*pNumPolygons);

				pIndices[*pNumPolygons]._1 = iBC;
				pIndices[*pNumPolygons]._2 = m_iCenterIndex;
				pIndices[*pNumPolygons]._3 = m_iCornerIndex[CORNER_RB];
				++(*pNumPolygons);
			}

			else
			{
				pIndices[*pNumPolygons]._1 = m_iCornerIndex[CORNER_LB];
				pIndices[*pNumPolygons]._2 = m_iCenterIndex;
				pIndices[*pNumPolygons]._3 = m_iCornerIndex[CORNER_RB];
				++(*pNumPolygons);
			}
		}

		if (true == isIn[CORNER_LT] ||
			true == isIn[CORNER_RT] ||
			true == isIn[CORNER_RB])
		{
			if (false == isDraw[NEIGHBOR_TOP])
			{
				pIndices[*pNumPolygons]._1 = m_iCornerIndex[CORNER_LT];
				pIndices[*pNumPolygons]._2 = iTC;
				pIndices[*pNumPolygons]._3 = m_iCenterIndex;
				++(*pNumPolygons);

				pIndices[*pNumPolygons]._1 = m_iCenterIndex;
				pIndices[*pNumPolygons]._2 = iTC;
				pIndices[*pNumPolygons]._3 = m_iCornerIndex[CORNER_RT];
				++(*pNumPolygons);
			}
			else
			{
				pIndices[*pNumPolygons]._1 = m_iCornerIndex[CORNER_LT];
				pIndices[*pNumPolygons]._2 = m_iCornerIndex[CORNER_RT];
				pIndices[*pNumPolygons]._3 = m_iCenterIndex;
				++(*pNumPolygons);
			}

			if (false == isDraw[NEIGHBOR_RIGHT])
			{
				pIndices[*pNumPolygons]._1 = m_iCornerIndex[CORNER_RT];
				pIndices[*pNumPolygons]._2 = iRC;
				pIndices[*pNumPolygons]._3 = m_iCenterIndex;
				++(*pNumPolygons);

				pIndices[*pNumPolygons]._1 = m_iCenterIndex;
				pIndices[*pNumPolygons]._2 = iRC;
				pIndices[*pNumPolygons]._3 = m_iCornerIndex[CORNER_RB];
				++(*pNumPolygons);
			}
			else
			{
				pIndices[*pNumPolygons]._1 = m_iCornerIndex[CORNER_RT];
				pIndices[*pNumPolygons]._2 = m_iCornerIndex[CORNER_RB];
				pIndices[*pNumPolygons]._3 = m_iCenterIndex;
				++(*pNumPolygons);
			}
		}

		return NOERROR;
	}


	if (true == pFrustum->is_InFrustum(pPosition[m_iCenterIndex], m_fRadius))
	{
		for (size_t i = 0; i < CHILD_END; ++i)
		{
			if (nullptr != m_pChild[i])
				m_pChild[i]->Culling_ToQuadTree(pFrustum, pPosition, pIndices, pNumPolygons);
		}
	}


	return NOERROR;
}

HRESULT CQuadTree::Make_Child(_vec3 * pPosition)
{
	_uint iLC, iTC, iRC, iBC;

	iLC = (m_iCornerIndex[CORNER_LT] + m_iCornerIndex[CORNER_LB]) >> 1;
	iTC = (m_iCornerIndex[CORNER_LT] + m_iCornerIndex[CORNER_RT]) >> 1;
	iRC = (m_iCornerIndex[CORNER_RT] + m_iCornerIndex[CORNER_RB]) >> 1;
	iBC = (m_iCornerIndex[CORNER_LB] + m_iCornerIndex[CORNER_RB]) >> 1;

	m_pChild[CHILD_LT] = CQuadTree::Create(m_pGraphic_Device, pPosition, m_iCornerIndex[CORNER_LT], iTC, m_iCenterIndex, iLC);
	m_pChild[CHILD_RT] = CQuadTree::Create(m_pGraphic_Device, pPosition, iTC, m_iCornerIndex[CORNER_RT], iRC, m_iCenterIndex);
	m_pChild[CHILD_RB] = CQuadTree::Create(m_pGraphic_Device, pPosition, m_iCenterIndex, iRC, m_iCornerIndex[CORNER_RB], iBC);
	m_pChild[CHILD_LB] = CQuadTree::Create(m_pGraphic_Device, pPosition, iLC, m_iCenterIndex, iBC, m_iCornerIndex[CORNER_LB]);

	return NOERROR;
}

HRESULT CQuadTree::Make_Neighbor()
{
	if (nullptr == m_pChild[CHILD_LT])
		return NOERROR;

	m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pChild[CHILD_RT];
	m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChild[CHILD_LB];

	if (nullptr != m_pNeighbor[NEIGHBOR_LEFT])
		m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RT];

	if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
		m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_LB];


	m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pChild[CHILD_LT];
	m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChild[CHILD_RB];

	if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
		m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LT];

	if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
		m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_RB];

	m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pChild[CHILD_LB];
	m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_TOP] = m_pChild[CHILD_RT];

	if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
		m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LB];

	if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
		m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_RT];

	m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pChild[CHILD_RB];
	m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_TOP] = m_pChild[CHILD_LT];


	if (nullptr != m_pNeighbor[NEIGHBOR_LEFT])
		m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RB];

	if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
		m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_LT];

	for (size_t i = 0; i < CHILD_END; ++i)
	{
		if (nullptr != m_pChild[i])
			m_pChild[i]->Make_Neighbor();
	}

	return NOERROR;
}

_bool CQuadTree::LevelofDetail(_vec3* pPosition)
{
	if (nullptr == m_pPipeLine)
		return false;

	if (nullptr == m_pChild[CHILD_LT])
		return true;

	_matrix	ViewMatrix = m_pPipeLine->Get_Transform(D3DTS_VIEW);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_vec3	vCamPosition = *(_vec3*)&ViewMatrix.m[3][0];

	_float	fDistance = D3DXVec3Length(&(pPosition[m_iCenterIndex] - vCamPosition));

	_float	fWidth = D3DXVec3Length(&(pPosition[m_iCornerIndex[CORNER_RT]] - pPosition[m_iCornerIndex[CORNER_LT]]));

	if (fDistance * 0.1f > fWidth)
		return true;

	return false;
}

CQuadTree * CQuadTree::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3* pPosition, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CQuadTree* pInstance = new CQuadTree(pGraphic_Device);

	if (FAILED(pInstance->Ready_QuadTree(pPosition, iNumVerticesX, iNumVerticesZ)))
	{
		MSG_BOX("Failed while Creating CQuadTree");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CQuadTree * CQuadTree::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3 * pPosition, _uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
	CQuadTree* pInstance = new CQuadTree(pGraphic_Device);

	if (FAILED(pInstance->Ready_QuadTree(pPosition, iLT, iRT, iRB, iLB)))
	{
		MSG_BOX("Failed while Creating CQuadTree");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuadTree::Free()
{
	for (size_t i = 0; i < CHILD_END; ++i)
		Safe_Release(m_pChild[i]);

	Safe_Release(m_pPipeLine);
	Safe_Release(m_pGraphic_Device);
}
