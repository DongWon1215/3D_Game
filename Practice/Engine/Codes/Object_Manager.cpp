#include "..\Headers\Object_Manager.h"
#include "..\Headers\Layer.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

const CComponent * CObject_Manager::Get_Component(_uint iSceneID, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (m_iNumScene <= iSceneID || 
		nullptr == m_pLayers)
		return nullptr;

	CLayer*	pLayer = Find_Layer(iSceneID, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;	

	return pLayer->Get_Component(pComponentTag, iIndex);
}

const CGameObject* CObject_Manager::Get_GameObject(_uint iSceneID, const _tchar * pLayerTag, _uint iIndex)
{
	if (CLayer* pLayer = Find_Layer(iSceneID, pLayerTag))
		return pLayer->Get_GameObject(iIndex);

	return nullptr;
}

const vector<CGameObject*> CObject_Manager::Get_ObjectList(_uint iSceneID, const _tchar * pLayerTag) {
	CLayer* pLayer = Find_Layer(iSceneID, pLayerTag);
	if (nullptr == pLayer)
		return vector<CGameObject*>();

	return pLayer->Get_ObjectList();
}

HRESULT CObject_Manager::Reserve_Object_Manager(_uint iNumScene)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumScene];

	m_iNumScene = iNumScene;

	return NOERROR;
}

HRESULT CObject_Manager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;
	
	if (nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;

	m_Prototypes.insert(PROTOTYPE::value_type(pPrototypeTag, pGameObject));

	return NOERROR;
}

HRESULT CObject_Manager::Add_GameObjectToLayer(const _tchar * pPrototypeTag, _uint iSceneIndex, const _tchar * pLayerTag, void* pArg)
{
	CGameObject*		pPrototype = Find_Prototype(pPrototypeTag);

	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*		pGameObject = pPrototype->Clone_GameObject(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*	pLayer = Find_Layer(iSceneIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		// 레이어에 객체를 추가한다.
		pLayer->Add_GameObject(pGameObject);

		// 준비된 레이어를 맵에 추가한다.
		m_pLayers[iSceneIndex].insert(LAYERS::value_type(pLayerTag, pLayer));
	}
	else
		pLayer->Add_GameObject(pGameObject);
	
	return NOERROR;
}

_int CObject_Manager::Update_GameObject_Manager(_double TimeDelta)
{
	for (size_t i = 0; i < m_iNumScene; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (nullptr != Pair.second)
				Pair.second->Update_Layer(TimeDelta);
		}
	}

	return _int();
}

_int CObject_Manager::LateUpdate_GameObject_Manager(_double TimeDelta)
{
	for (size_t i = 0; i < m_iNumScene; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (nullptr != Pair.second)
				Pair.second->LateUpdate_Layer(TimeDelta);
		}
	}

	return _int();
}

HRESULT CObject_Manager::Clear_Scene(_uint iSceneID)
{
	if (m_iNumScene <= iSceneID)
		return E_FAIL;

	for (auto& Pair : m_pLayers[iSceneID])
	{
		Safe_Release(Pair.second);
	}

	m_pLayers[iSceneID].clear();

	return NOERROR;
}


CGameObject * CObject_Manager::Find_Prototype(const _tchar * pPrototypeTag)
{
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder(pPrototypeTag));

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

CLayer * CObject_Manager::Find_Layer(_uint iSceneIndex, const _tchar * pLayerTag)
{
	if (m_iNumScene <= iSceneIndex)
		return nullptr;

	auto	iter = find_if(m_pLayers[iSceneIndex].begin(), m_pLayers[iSceneIndex].end(), CTag_Finder(pLayerTag));

	if (iter == m_pLayers[iSceneIndex].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (auto& Pair : m_Prototypes)
	{
		Safe_Release(Pair.second);
	}

	m_Prototypes.clear();


	for (size_t i = 0; i < m_iNumScene; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Safe_Release(Pair.second);
		}
		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	
}
