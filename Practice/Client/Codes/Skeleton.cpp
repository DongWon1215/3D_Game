#include "stdafx.h"
#include "..\Headers\Skeleton.h"
#include "Management.h"
#include "Player.h"
#include "Bullet.h"


CSkeleton::CSkeleton(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CSkeleton::CSkeleton(const CSkeleton & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkeleton::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkeleton::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(rand() % 129, 0.f, rand() % 129));
	SetCollider();

	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	m_iAtt = 10;
	m_iCurHp = 100;

	Safe_AddRef(pManagement);

	m_pPlayer = (CPlayer*)pManagement->Get_GameObject(L"GameObject_Player", SCENE_STAGE, L"Layer_Player", 0);
	Safe_AddRef(m_pPlayer);

	Safe_Release(pManagement);

	return S_OK;
}

_int CSkeleton::Update_GameObject(_double TimeDelta)
{
	m_iCurHp;

	m_TimeDelta = TimeDelta;

	SetUp_OnTerrain();

	m_pColliderCom->Update_Collider(TimeDelta);
	m_pSwordCollider->Update_Collider(TimeDelta);

	IsCanAttack(TimeDelta);

	m_bisFinish = m_pMeshCom->is_Finished(0.0f);

	ChasePlayer(TimeDelta);

	if (m_pPlayer->Get_PlayerState() == STATE_ATTACK)
		IsCollision(m_pPlayer->Get_Ray(), m_pPlayer->Get_Player_HeadPos());

	if (((CCollider*)m_pPlayer->Get_Component(L"Com_Collider"))->Collision_AABB(m_pColliderCom) && IsCanAttack(TimeDelta) && m_fCoolTime <= 0.f)
	{
		m_pPlayer->Set_Hp(m_iAtt);
		m_pPlayer->Set_PlayerAttacked(true);
	}

	if (m_pMeshCom->is_Finished() && m_iCurHp <= 0)
		m_bIsDead = true;

	return _int();
}

_int CSkeleton::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_MatWeapon = *m_pMatWeapon * *m_pTransformCom->Get_WorldMatrixPointer();

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CSkeleton::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pMeshCom)
		return E_FAIL;

	m_pMeshCom->Play_AnimationSet(m_TimeDelta);

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader();
	m_pShaderCom->Begin_Pass(0);

	_uint		iNumMeshContainer = m_pMeshCom->Get_NumMeshContainer();

	for (size_t i = 0; i < iNumMeshContainer; ++i)
	{
		_ulong		dwNumSubset = m_pMeshCom->Get_NumSubset(i);

		m_pMeshCom->Update_SkinnedMesh(i);

		for (_ulong j = 0; j < dwNumSubset; ++j)
		{
			if (FAILED(m_pShaderCom->Set_Texture("g_DiffuseTexture", m_pMeshCom->Get_MaterialTexture(i, j, MESHTEXTURE::TYPE_DIFFUSE))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Set_Texture("g_NormalTexture", m_pMeshCom->Get_MaterialTexture(i, j, MESHTEXTURE::TYPE_NORMAL))))
				return E_FAIL;

			m_pShaderCom->Commit_Change();

			m_pMeshCom->Render_Mesh(i, j);
		}
	}

	m_pShaderCom->End_Pass();
	m_pShaderCom->End_Shader();

#ifdef _DEBUG
		if (FAILED(m_pColliderCom->Render_Collider()))
			return E_FAIL;
		if (FAILED(m_pSwordCollider->Render_Collider()))
			return E_FAIL;
#endif

	return S_OK;
}

HRESULT CSkeleton::Add_Component()
{
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Mesh_Skeleton", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Transform
	CTransform::STATEDESC		StateDesc;
	StateDesc.fSpeedPerSec = 5.f;
	StateDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &StateDesc)))
		return E_FAIL;

	// For.Com_Navigation
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Stage_Navigation", L"Com_Navigation", (CComponent**)&m_pNavigationCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkeleton::SetUp_ConstantTable()
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

HRESULT CSkeleton::SetUp_OnTerrain()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	const CVIBuffer_Terrain* pTerrainBuffer = (const CVIBuffer_Terrain*)pManagement->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
	{
		Safe_Release(pManagement);
		return -1;
	}

	_vec3		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = pTerrainBuffer->Compute_Y(vPosition);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	Safe_Release(pManagement);

	return S_OK;
}

_bool CSkeleton::IsCollision(_vec3 vRay, _vec3 vPosition)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return false;
	Safe_AddRef(pManagement);
	if (m_pColliderCom->Collision_Ray(&vRay, &vPosition))
	{
		vector<CGameObject*> pList = (vector<CGameObject*>)pManagement->Get_ObjectList(SCENE_STAGE, L"Layer_Bullet");
		for (auto& pDest : pList)
			if (SetAnimation((CCollider*)(dynamic_cast<CBullet*>(pDest))->Get_Component(L"Com_Collider")))
			{
				dynamic_cast<CBullet*>(pDest)->SetisHit(true);
			}
	}
	else
		m_bIsHit = false;

	Safe_Release(pManagement);


	return false;
}

_bool CSkeleton::IsCanAttack(_double TimeDelta)
{
	if (m_fCoolTime <= 0.f)
	{
		m_fCoolTime = 0.f;
		return true;
	}

	m_fCoolTime -= 1.f * TimeDelta;

	return false;
}

void CSkeleton::SetCollider()
{

	// For.Com_Collider_AABB
	CCollider::COLLIDER_DESC	Collider_Desc;
	Collider_Desc.vLocalScale = _vec3(1.f, 2.f, 1.f);
	Collider_Desc.vLocalPosition = _vec3(0.f, 1.f, 0.f);
	Collider_Desc.pTargetMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider", L"Com_Collider_HitBox", (CComponent**)&m_pColliderCom, &Collider_Desc)))
		return;

	m_pMatWeapon = &m_pMeshCom->Get_FrameDerived("Bip01-L-Finger2Nub")->CombinedTransformationMatrix;

	CCollider::COLLIDER_DESC	WeaponCollider_Desc;
	WeaponCollider_Desc.vLocalScale = _vec3(1.f, 1.f, 1.f);
	WeaponCollider_Desc.vLocalPosition = _vec3(0.f, 0.f, 0.f);
	WeaponCollider_Desc.pTargetMatrix = &m_MatWeapon;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider", L"Com_Collider_AttackBox", (CComponent**)&m_pSwordCollider, &Collider_Desc)))
		return;
	

}

void CSkeleton::ChasePlayer(_double TimeDelta)
{
	if (D3DXVec3Length(&(m_pPlayer->Get_PlayerPos() - m_pTransformCom->Get_State(CTransform::STATE_POSITION))) <= 10.f &&
		D3DXVec3Length(&(m_pPlayer->Get_PlayerPos() - m_pTransformCom->Get_State(CTransform::STATE_POSITION))) > 1.5f &&
		!m_bIsHit)
	{
		m_pTransformCom->Go_Target(m_pNavigationCom, m_pPlayer->Get_PlayerPos(), TimeDelta * 0.85f);
	}

	else if (D3DXVec3Length(&(m_pPlayer->Get_PlayerPos() - m_pTransformCom->Get_State(CTransform::STATE_POSITION))) <= 1.5f &&
		!m_bIsHit && IsCanAttack(TimeDelta))
	{
		if (m_pMeshCom->is_Finished())
		{
			m_eCurState = STATE_ATTACK1;
			m_fCoolTime = 1.5f;
		}
	}

	else if (!m_bIsHit || !IsCanAttack(TimeDelta))
		m_eCurState = STATE_IDLE;

}

void CSkeleton::ConnectAnimation()
{
}

_bool CSkeleton::SetAnimation(CCollider* pCollider)
{
	if (m_ePreState != m_eCurState)
	{

	}
	if (m_pColliderCom->Collision_Sphere(pCollider))
	{
		if (m_iCurHp <= 0)
		{
			m_pMeshCom->SetUp_AnimationSet(18);

			return true;
		}

		m_pMeshCom->SetUp_AnimationSet(8);
		switch (m_pPlayer->Get_PlayerWeapon())
		{
		case WEAPON_PISTOL:
			m_iCurHp -= 5;
			break;
		case WEAPON_RIFFLE:
			m_iCurHp -= 6;
			break;
		case WEAPON_SNIPER:
			m_iCurHp -= 25;
			break;
		}
		m_bIsHit = true;

		return true;
	}

	return false;
}

CSkeleton * CSkeleton::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSkeleton* pInstance = new CSkeleton(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CSkeleton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkeleton::Clone_GameObject(void * pArg)
{
	CSkeleton* pInstance = new CSkeleton(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CSkeleton Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeleton::Free()
{
	
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pPlayer);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pSwordCollider);

	CGameObject::Free();
}
