#pragma once

// 1. 클라이언트에 존재하는 게임오브젝트들의 부모가 되는클래스다.

#include "Base.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	LPDIRECT3DDEVICE9 Get_Graphic_Device() const {
		return m_pDevice; }
	const CComponent* Get_Component(const _tchar* pComponentTag) const;
	_float Get_ViewZ() const {
		return m_fViewZ;
	}
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();
	_bool GetIsDead() const { return m_bIsDead; }
protected: // 원형을 검색해서 복제하고 멤버변수에 대입도해주고 컨테이너에 보관하는 기능까지 다해.
	HRESULT Add_Component(_uint iSceneID, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppComponent, void* pArg = nullptr);
	HRESULT Compute_ViewZ(_vec3 vWorldPos);
private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
private:
	_float					m_fViewZ = 0.f;
protected:
	_bool					m_bIsDead = false;
private:
	unordered_map<const _tchar*, CComponent*>			m_Components;
	typedef unordered_map<const _tchar*, CComponent*>	COMPONENTS;
private:
	// 임의의 컴포넌트가 있는지에 대해 조사
	CComponent* Find_Component(const _tchar* pComponentTag) const;
public:
	virtual CGameObject* Clone_GameObject(void* pArg) = 0; // 복제하여 생성하낟.
	virtual void Free();
};

END
