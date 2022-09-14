#include "..\Headers\Navigation.h"
#include "..\Headers\Cell.h"


CNavigation::CNavigation(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
{
}

CNavigation::CNavigation(const CNavigation & rhs)
	:CComponent(rhs)
	, m_iNumCells(rhs.m_iNumCells)
	, m_Cells(rhs.m_Cells)
{
	for (auto& pIter : m_Cells)
		Safe_AddRef(pIter);
}

HRESULT CNavigation::Ready_Component_Prototype(const _tchar * pFilePath)
{
	HANDLE hFile;
	_ulong dwByte = 0;

	hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	ReadFile(hFile, &m_iNumCells, sizeof(_uint), &dwByte, nullptr);

	m_Cells.reserve(m_iNumCells);

	for (size_t i = 0; i < m_iNumCells; i++)
	{
		_vec3 Vector[3];

		ReadFile(hFile, &Vector, sizeof(_vec3) * 3, &dwByte, nullptr);

		if (0 == hFile)
			return E_FAIL;

		CCell* pCell = CCell::Create(Get_Graphic_Device(), Vector[0], Vector[1], Vector[2], i);

		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	if (FAILED(Set_Neighbor()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigation::Ready_Component(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_iCurrentIndex, pArg, sizeof(_uint));

	return S_OK;
}

_bool CNavigation::is_Move(const _vec3 * pResultPos)
{
	CCell* pNeighbor = nullptr;
	if (m_Cells[m_iCurrentIndex]->Check_Move(pResultPos, &pNeighbor))
	{
		return true;
	}
	else
	{
		if (pNeighbor != nullptr)
		{
			m_iCurrentIndex = pNeighbor->Get_Index();
			return true;
		}
		else
			return false;
	}

	return _bool();
}

void CNavigation::Setting_Index(_vec3* TargetPos)
{
	_int Index = 0;
	CCell::LINE eLine;
	for (auto& piter : m_Cells)
	{
		if (piter->Is_In(TargetPos, &eLine))
			m_iCurrentIndex = Index;

		Index++;
	}
}

#ifdef _DEBUG
HRESULT CNavigation::Render_Navigation()
{
	for (auto& pIter : m_Cells)
	{
		if(nullptr != pIter)
		pIter->Render_Cell();
	}

	return S_OK;
}
#endif

HRESULT CNavigation::Set_Neighbor()
{

	for (size_t i = 0; i < m_iNumCells; i++)
	{
		for (size_t j = 0; j < m_iNumCells; j++)
		{
			if (m_Cells[i] == m_Cells[j])
				continue;

			if (m_Cells[j]->Compare_Point(m_Cells[i]->Get_Point(CCell::POINT_A), m_Cells[i]->Get_Point(CCell::POINT_B)))
				m_Cells[i]->SetNeighbor(CCell::NEIGHBOR_AB, m_Cells[j]);

			if (m_Cells[j]->Compare_Point(m_Cells[i]->Get_Point(CCell::POINT_B), m_Cells[i]->Get_Point(CCell::POINT_C)))
				m_Cells[i]->SetNeighbor(CCell::NEIGHBOR_BC, m_Cells[j]);

			if (m_Cells[j]->Compare_Point(m_Cells[i]->Get_Point(CCell::POINT_C), m_Cells[i]->Get_Point(CCell::POINT_A)))
				m_Cells[i]->SetNeighbor(CCell::NEIGHBOR_CA, m_Cells[j]);
		}
	}

	return S_OK;
}

CNavigation * CNavigation::Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pFilePath)
{
	CNavigation* pInstance = new CNavigation(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype(pFilePath)))
	{
		MSG_BOX("Failed while Creating CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CNavigation::Clone_Component(void * pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed while Creating CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	for (auto& Iter : m_Cells)
		Safe_Release(Iter);

	m_Cells.clear();
	m_Cells.shrink_to_fit();

	CComponent::Free();
}
