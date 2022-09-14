#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CMesh_Static;
END

class CObject_Tool final : public CGameObject
{
private:
	explicit CObject_Tool(LPDIRECT3DDEVICE9 pDevice);
	explicit CObject_Tool(CObject_Tool& rhs);
	virtual ~CObject_Tool() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_OnTerrain();

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CMesh_Static*			m_pMeshCom = nullptr;

private:
	bool m_bIsPicking = false;

public:
	// CGameObject을(를) 통해 상속됨
	static CObject_Tool* Create(LPDIRECT3DDEVICE9 pDevice); // 원본을 생성한다.
	virtual void Free();
	virtual CGameObject * Clone_GameObject(void * pArg) override;
};

