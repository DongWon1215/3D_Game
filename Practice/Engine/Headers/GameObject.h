#pragma once

// 1. Ŭ���̾�Ʈ�� �����ϴ� ���ӿ�����Ʈ���� �θ� �Ǵ�Ŭ������.

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
protected: // ������ �˻��ؼ� �����ϰ� ��������� ���Ե����ְ� �����̳ʿ� �����ϴ� ��ɱ��� ����.
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
	// ������ ������Ʈ�� �ִ����� ���� ����
	CComponent* Find_Component(const _tchar* pComponentTag) const;
public:
	virtual CGameObject* Clone_GameObject(void* pArg) = 0; // �����Ͽ� �����ϳ�.
	virtual void Free();
};

END
