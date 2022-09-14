#pragma once
#include "Client_Extend.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CCollider;
END

class CPlayer;
class CBullet final : public CGameObject
{
private:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	void SetisHit(_bool IsHit);
private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;	
	CShader*				m_pShaderCom = nullptr;
private:
	_bool					m_bIsHIt = false;
	_bool m_bIsShow = false;
	_float					m_fLifeTime = 10.f;

	_vec3					m_vRayDIr;
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
public:
	static CBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

