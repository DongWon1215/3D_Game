#pragma once
#include "Client_Extend.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CTransform;
class CMesh_Dynamic;
END


class CPistol final : public CGameObject
{
private:
	explicit CPistol(LPDIRECT3DDEVICE9 pDevice);
	explicit CPistol(const CPistol& rhs);
	virtual ~CPistol() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	_float Get_CurBullet() { return m_iCurrentBullet; }

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CMesh_Dynamic*			m_pMeshCom = nullptr;

private:
	_matrix*				m_pHandMatrix;
	_matrix*				m_pPlayerWorldMatrix;
	_matrix					m_RenderingMatrix;
	_double					m_TimeDelta;
	_bool					m_bIsFrameStart = false;
	_vec3					m_vTargetPos;

private:
	_float					m_iMaxBullet;
	_float					m_iCurrentBullet;
	_float					m_fCooltime = 0.f;
private:
	STATE					m_ePlayerState;
	WEAPON					m_eWeapon;

	SCENEID					m_eScene;						  //ÇöÀç ¾À ³Ñ¹ö

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CPistol* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};