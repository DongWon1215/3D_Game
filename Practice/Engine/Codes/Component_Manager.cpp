#include "..\Headers\Component_Manager.h"

#include "..\Headers\Component.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Reserve_Component_Manager(_uint iNumScene)
{
	if (nullptr != m_pPrototypes)
		return E_FAIL;

	m_pPrototypes = new PROTOTYPES[iNumScene];

	m_iNumScene = iNumScene;

	return NOERROR;
}

HRESULT CComponent_Manager::Add_Prototype(_uint iSceneID, const _tchar * pPrototypeTag, CComponent * pComponent)
{
	if (nullptr == pComponent)
		return E_FAIL;

	CComponent*	pPrototype = Find_Prototype(iSceneID, pPrototypeTag);

	if (nullptr != pPrototype)
		return E_FAIL;

	m_pPrototypes[iSceneID].insert(PROTOTYPES::value_type(pPrototypeTag, pComponent));

	return NOERROR;
}

CComponent * CComponent_Manager::Clone_Component(_uint iSceneID, const _tchar * pPrototypeTag, void* pArg)
{
	CComponent*	pPrototype = Find_Prototype(iSceneID, pPrototypeTag);

	if (nullptr == pPrototype)
		return nullptr;

	CComponent* pComponent = pPrototype->Clone_Component(pArg);
	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

HRESULT CComponent_Manager::Clear_Scene(_uint iSceneID)
{
	if (m_iNumScene <= iSceneID)
		return E_FAIL;

	for (auto& Pair : m_pPrototypes[iSceneID])
	{
		Safe_Release(Pair.second);
	}

	m_pPrototypes[iSceneID].clear();

	return NOERROR;
}

CComponent * CComponent_Manager::Find_Prototype(_uint iSceneID, const _tchar * pPrototypeTag)
{
	if (m_iNumScene <= iSceneID)
		return nullptr;

	auto	iter = find_if(m_pPrototypes[iSceneID].begin(), m_pPrototypes[iSceneID].end(), CTag_Finder(pPrototypeTag));

	if (iter == m_pPrototypes[iSceneID].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free()
{
	for (size_t i = 0; i < m_iNumScene; ++i)
	{
		for (auto& Pair : m_pPrototypes[i])
		{
			Safe_Release(Pair.second);
		}
		m_pPrototypes[i].clear();
	}
	Safe_Delete_Array(m_pPrototypes);
}
