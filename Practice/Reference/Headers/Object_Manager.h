#pragma once

// 1. 게임내에 사용될 오브젝트들의 원형을 보관한다.

#include "Base.h"
#include "GameObject.h"
BEGIN(Engine)

class CLayer;
class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	const CComponent* Get_Component(_uint iSceneID, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	const CGameObject* Get_GameObject(_uint iSceneID, const _tchar* pLayerTag, _uint iIndex = 0);
	const vector<CGameObject*> Get_ObjectList(_uint iSceneID, const _tchar* pLayerTag);
public:
	HRESULT Reserve_Object_Manager(_uint iNumScene);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pGameObject);
	HRESULT Add_GameObjectToLayer(const _tchar* pPrototypeTag, _uint iSceneIndex, const _tchar* pLayerTag, void* pArg);
	_int Update_GameObject_Manager(_double TimeDelta);
	_int LateUpdate_GameObject_Manager(_double TimeDelta);
	HRESULT Clear_Scene(_uint iSceneID);
private:
	unordered_map<const _tchar*, CGameObject*>			m_Prototypes;
	typedef unordered_map<const _tchar*, CGameObject*>	PROTOTYPE;
private:	
	unordered_map<const _tchar*, CLayer*>*				m_pLayers = nullptr;
	typedef unordered_map<const _tchar*, CLayer*>		LAYERS;
private:
	_uint		m_iNumScene;

private:
	CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	CLayer* Find_Layer(_uint iSceneIndex, const _tchar* pLayerTag);
public:
	virtual void Free();
};

END