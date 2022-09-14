#include "stdafx.h"
#include "..\Headers\Bullet.h"
#include "Management.h"
#include "Player.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CBullet::CBullet(const CBullet & rhs)
	:CGameObject(rhs)
{
}

HRESULT CBullet::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBullet::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	if(nullptr != pArg)
	m_vRayDIr = *(_vec3*)pArg;

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vRayDIr);

	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return false;

	Safe_AddRef(pManagement);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(L"GameObject_Player", SCENE_STAGE, L"Layer_Player", 0));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pPlayer->Get_Player_HeadPos());

	Safe_Release(pManagement);
	return S_OK;
}

_int CBullet::Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pTransformCom ||
		nullptr == m_pColliderCom)
		return E_FAIL;

	if (m_bIsHIt || m_fLifeTime <= 0.f)
		m_bIsDead = true;

	m_fLifeTime -= 0.1f;

	m_pColliderCom->Update_Collider(TimeDelta);

	m_pTransformCom->Go_Straight(TimeDelta);
	CManagement* pManagement = CManagement::GetInstance();
	Safe_AddRef(pManagement);
	if (pManagement->KeyDown(KEY_UP))
		m_bIsShow = !m_bIsShow;
		Safe_Release(pManagement);

	return _int();
}

_int CBullet::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
		
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CBullet::Render_GameObject()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader();

	m_pShaderCom->Begin_Pass(0);
	
	m_pShaderCom->End_Pass();

	m_pShaderCom->End_Shader();


#ifdef _DEBUG
	//if(m_bIsShow)
	//m_pColliderCom->Render_Collider();
#endif // _DEBUG

	return S_OK;
}

void CBullet::SetisHit(_bool IsHit)
{ 
	m_bIsHIt = IsHit;
}


HRESULT CBullet::Add_Component()
{
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Transform
	CTransform::STATEDESC		StateDesc;
	StateDesc.fSpeedPerSec = 5.f;
	StateDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &StateDesc)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDER_DESC	Collider_Desc;
	Collider_Desc.vLocalScale = _vec3(1.f, 1.f, 1.f);
	Collider_Desc.vLocalPosition = _vec3(0.f, 0.f, 0.f);
	Collider_Desc.fRadius = 0.2f;
	Collider_Desc.pTargetMatrix = m_pTransformCom->Get_WorldMatrixPointer();

	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_SPHERE", L"Com_Collider", (CComponent**)&m_pColliderCom, &Collider_Desc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBullet::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom)
		return E_FAIL;

	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Value("g_matView", &pManagement->Get_Transform(D3DTS_VIEW), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Value("g_matProj", &pManagement->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix))))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBullet* pInstance = new CBullet(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBullet::Clone_GameObject(void * pArg)
{
	CBullet* pInstance = new CBullet(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CBullet_Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
