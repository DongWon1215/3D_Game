#pragma once

// 1. ��ü���� ��Ƽ� �����Ѵ�.

#include "Base.h"

BEGIN(Engine)

class CComponent;
class CGameObject;
class CLayer final : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;
public:
	const CComponent* Get_Component(const _tchar* pComponentTag, _uint iIndex = 0);
	const CGameObject* Get_GameObject(_uint iIndex = 0);
	const vector<CGameObject*> Get_ObjectList() const;
public:
	HRESULT Ready_Layer();
	HRESULT Add_GameObject(CGameObject* pGameObject);
	_int Update_Layer(_double TimeDelta);
	_int LateUpdate_Layer(_double TimeDelta);
private:
	vector<CGameObject*>			m_ObjectList;
	typedef vector<CGameObject*>	OBJECTLIST;
public:
	static CLayer* Create();
	virtual void Free();
};

END