#pragma once

#include "Client_Extend.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CTransform;
class CMesh_Static;
END

BEGIN(Client)

class CBurning_Car final : public CGameObject
{
private:
	explicit CBurning_Car(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBurning_Car(const CBurning_Car& rhs);
	virtual ~CBurning_Car() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();
private:
	CShader*				m_pShaderCom = nullptr;	
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CMesh_Static*			m_pMeshCom = nullptr;
private:
	_double					m_TimeDelta;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_OnTerrain();
public:
	static CBurning_Car* Create(LPDIRECT3DDEVICE9 pGraphic_Device); // ������ �����Ѵ�.
	virtual CGameObject* Clone_GameObject(void* pArg); // �������� �����Ѵ�.
	virtual void Free();
};

END