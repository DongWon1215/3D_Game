#include "stdafx.h"
#include "..\Headers\Monster.h"
#include "Management.h"
#include "Player.h"
#include "Bullet.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CMonster::CMonster(const CMonster& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CMonster::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(rand() % 240 + 10, 0.f, rand() % 240 + 10));
	SetCollider();

	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	m_pPlayer = (CPlayer*)pManagement->Get_GameObject(L"GameObject_Player", SCENE_STAGE, L"Layer_Player", 0);
	Safe_AddRef(m_pPlayer);

	m_pNavigationCom->Setting_Index(&m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	Safe_Release(pManagement);

	return NOERROR;
}

_int CMonster::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	SetUp_OnTerrain(); 

	if (m_iCurHp <= 0)
	{
		m_pMeshCom->SetUp_AnimationSet(18);

		if (m_pMeshCom->is_Finished())
			m_bIsDead = true;
	}

	for (int i = 0; i < HITBOX_END; i++)
	{
		m_pColliderCom[i]->Update_Collider(TimeDelta);
	}

	IsCanAttack(TimeDelta);

	if (m_pMeshCom->is_Finished() && m_iCurHp > 0)
		m_pMeshCom->SetUp_AnimationSet(0);

	ChasePlayer(TimeDelta);


	if (m_pPlayer->Get_PlayerState() == STATE_ATTACK)
	{
		IsCollision();
	}

	if (((CCollider*)m_pPlayer->Get_Component(L"Com_Collider"))->Collision_AABB(m_pColliderCom[HITBOX_LEFTARM]) && IsCanAttack(TimeDelta) && m_fCoolTime <= 0.f ||
		((CCollider*)m_pPlayer->Get_Component(L"Com_Collider"))->Collision_AABB(m_pColliderCom[HITBOX_RIGHTARM]) && IsCanAttack(TimeDelta) && m_fCoolTime <= 0.f)
	{
		m_pPlayer->Set_Hp(m_iAtt);
		m_pPlayer->Set_PlayerAttacked(true);
	}

	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);


	Safe_Release(pManagement);

	return _int();
}

_int CMonster::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);


	return _int();
}

HRESULT CMonster::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pMeshCom)
		return E_FAIL;

	m_pMeshCom->Play_AnimationSet(m_TimeDelta);

	UpdateCollider();

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
	if (m_bIsShow)
	{
		for (int i = 0; i < HITBOX_END; i++)
		{
			if (FAILED(m_pColliderCom[i]->Render_Collider()))
				return E_FAIL;
		}
	}
#endif

	return NOERROR;
}

HRESULT CMonster::Add_Component()
{
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;	

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Mesh_Minion", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
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

	return NOERROR;
}

HRESULT CMonster::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom)
		return E_FAIL;

	CManagement* pManagement = CManagement::GetInstance();
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

	return NOERROR;
}

HRESULT CMonster::SetUp_OnTerrain()
{
	CManagement* pManagement = CManagement::GetInstance();
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

	return NOERROR;
}

HRESULT CMonster::Set_Collider_Show()
{	
	m_bIsShow = !m_bIsShow;

	return S_OK;
}

_bool CMonster::IsCollision()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return false;
	Safe_AddRef(pManagement);
	//if (m_pColliderCom[HITBOX_RANGE]->Collision_Ray(&vRay, &vPosition))
	//{
		vector<CGameObject*> pList = (vector<CGameObject*>)pManagement->Get_ObjectList(SCENE_STAGE, L"Layer_Bullet");
		for (auto& pDest : pList)
			if (SetAnimation((CCollider*)(dynamic_cast<CBullet*>(pDest))->Get_Component(L"Com_Collider")))
			{
				dynamic_cast<CBullet*>(pDest)->SetisHit(true);
				Safe_Release(pManagement);
				return true;
			}
	//}
	//else
	//	m_bIsHit = false;

	Safe_Release(pManagement);


	return false;
}

_bool CMonster::IsCanAttack(_double TimeDelta)
{
	if (m_fCoolTime <= 0.f)
	{
		m_fCoolTime = 0.f;
		return true;
	}

	m_fCoolTime -= 0.7f * TimeDelta;

	return false;
}

void CMonster::SetCollider()
{

	// For.Com_Collider_AABB
	//CCollider::COLLIDER_DESC	Collider_Desc;
	//Collider_Desc.vLocalScale = _vec3(1.f, 2.f, 1.f);
	//Collider_Desc.vLocalPosition = _vec3(0.f, 1.f, 0.f);
	//Collider_Desc.pTargetMatrix = m_pTransformCom->Get_WorldMatrixPointer();

	// For.Com_Collider_SPHERE
	m_matRightCalf = &m_pMeshCom->Get_FrameDerived("RightLeg")->CombinedTransformationMatrix;
	m_RLegCollider_Desc;
	m_RLegCollider_Desc.vLocalScale = _vec3(1.f, 1.f, 1.f);
	m_RLegCollider_Desc.vLocalPosition = _vec3(0.f, 0.f, 0.f);
	m_RLegCollider_Desc.fRadius = 0.5f;
	m_RLegCollider_Desc.pTargetMatrix = &m_matRealMtx[HITBOX_RIGHTCLAFT];

	m_matLeftCalf = &m_pMeshCom->Get_FrameDerived("LeftLeg")->CombinedTransformationMatrix;
	m_LLegCollider_Desc;
	m_LLegCollider_Desc.vLocalScale = _vec3(1.f, 1.f, 1.f);
	m_LLegCollider_Desc.vLocalPosition = _vec3(0.f, 0.f, 0.f);
	m_LLegCollider_Desc.fRadius = 0.5f;
	m_LLegCollider_Desc.pTargetMatrix = &m_matRealMtx[HITBOX_LEFTCLAFT];

	m_matRightArm = &m_pMeshCom->Get_FrameDerived("RightHand")->CombinedTransformationMatrix;
	m_RArmCollider_Desc;
	m_RArmCollider_Desc.vLocalScale = _vec3(1.f, 1.f, 1.f);
	m_RArmCollider_Desc.vLocalPosition = _vec3(0.f, 0.f, 0.f);
	m_RArmCollider_Desc.fRadius = 0.5f;
	m_RArmCollider_Desc.pTargetMatrix = &m_matRealMtx[HITBOX_RIGHTARM];

	m_matLeftArm = &m_pMeshCom->Get_FrameDerived("LeftHand")->CombinedTransformationMatrix;
	m_LArmCollider_Desc;
	m_LArmCollider_Desc.vLocalScale = _vec3(1.f, 1.f, 1.f);
	m_LArmCollider_Desc.vLocalPosition = _vec3(0.f, 0.f, 0.f);
	m_LArmCollider_Desc.fRadius = 0.5f;
	m_LArmCollider_Desc.pTargetMatrix = &m_matRealMtx[HITBOX_LEFTARM];

	m_matHead = &m_pMeshCom->Get_FrameDerived("head")->CombinedTransformationMatrix;
	m_HeadCollider_Desc;
	m_HeadCollider_Desc.vLocalScale = _vec3(1.f, 1.f, 1.f);
	m_HeadCollider_Desc.vLocalPosition = _vec3(0.f, 0.f, 0.f);
	m_HeadCollider_Desc.fRadius = 0.5f;
	m_HeadCollider_Desc.pTargetMatrix = &m_matRealMtx[HITBOX_HEAD];

	m_matBody = &m_pMeshCom->Get_FrameDerived("Spine")->CombinedTransformationMatrix;
	m_BodyCollider_Desc;
	m_BodyCollider_Desc.vLocalScale = _vec3(1.f, 1.f, 1.f);
	m_BodyCollider_Desc.vLocalPosition = _vec3(0.f, 0.f, 0.f);
	m_BodyCollider_Desc.fRadius = 0.5f;
	m_BodyCollider_Desc.pTargetMatrix = &m_matRealMtx[HITBOX_BODY];

	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_SPHERE", L"Com_Collider_RightLeg", (CComponent**)&m_pColliderCom[HITBOX_RIGHTCLAFT], &m_RLegCollider_Desc)))
		return;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_SPHERE", L"Com_Collider_LeftLeg", (CComponent**)&m_pColliderCom[HITBOX_LEFTCLAFT], &m_LLegCollider_Desc)))
		return;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_SPHERE", L"Com_Collider_RightArm", (CComponent**)&m_pColliderCom[HITBOX_RIGHTARM], &m_RArmCollider_Desc)))
		return;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_SPHERE", L"Com_Collider_LeftArm", (CComponent**)&m_pColliderCom[HITBOX_LEFTARM], &m_LArmCollider_Desc)))
		return;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_SPHERE", L"Com_Collider_Body", (CComponent**)&m_pColliderCom[HITBOX_BODY], &m_BodyCollider_Desc)))
		return;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_SPHERE", L"Com_Collider_Head", (CComponent**)&m_pColliderCom[HITBOX_HEAD], &m_HeadCollider_Desc)))
		return;
	//if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider", L"Com_Collider_HitBox", (CComponent**)&m_pColliderCom[HITBOX_RANGE], &Collider_Desc)))
	//	return;

}

void CMonster::ChasePlayer(_double TimeDelta)
{
	if (D3DXVec3Length(&(m_pPlayer->Get_PlayerPos() - m_pTransformCom->Get_State(CTransform::STATE_POSITION))) <= 10.f &&
		D3DXVec3Length(&(m_pPlayer->Get_PlayerPos() - m_pTransformCom->Get_State(CTransform::STATE_POSITION))) > 1.5f && 
		!m_bIsHit)
	{
		m_pTransformCom->Go_Target(m_pNavigationCom, m_pPlayer->Get_PlayerPos(), TimeDelta * 0.75f);
		m_pMeshCom->SetUp_AnimationSet(15);
	}

	else if (D3DXVec3Length(&(m_pPlayer->Get_PlayerPos() - m_pTransformCom->Get_State(CTransform::STATE_POSITION))) <= 1.5f && IsCanAttack(TimeDelta) &&
		!m_bIsHit)
	{
		if (m_pMeshCom->is_Finished())
			m_pMeshCom->SetUp_AnimationSet(5);

		if (m_pMeshCom->is_Finished())
		m_fCoolTime = 1.5f;

	}

	else if (!IsCanAttack(TimeDelta) &&	!m_bIsHit)
		m_pMeshCom->SetUp_AnimationSet(0);

}

_bool CMonster::SetAnimation(CCollider* pCollider)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return false;

	if (m_pMeshCom->is_Finished())
		m_bIsHit = false;

	if (m_pColliderCom[HITBOX_BODY]->Collision_Sphere(pCollider))
	{

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
		pManagement->Add_GameObjectToLayer(L"GameObject_Bleeding", SCENE_STAGE, L"Layer_Effect", &(_vec3)m_matRealMtx[HITBOX_BODY].m[3]);

		return true;
	}

	else if (m_pColliderCom[HITBOX_HEAD]->Collision_Sphere(pCollider))
	{
		m_pMeshCom->SetUp_AnimationSet(9);
		switch (m_pPlayer->Get_PlayerWeapon())
		{
		case WEAPON_PISTOL:
			m_iCurHp -= 10;
			break;
		case WEAPON_RIFFLE:
			m_iCurHp -= 12;
			break;
		case WEAPON_SNIPER:
			m_iCurHp -= 50;
			break;

		}
		m_bIsHit = true;
		pManagement->Add_GameObjectToLayer(L"GameObject_Bleeding", SCENE_STAGE, L"Layer_Effect", &(_vec3)m_matRealMtx[HITBOX_HEAD].m[3]);

		return true;
	}

	else if (m_pColliderCom[HITBOX_LEFTARM]->Collision_Sphere(pCollider))
	{
		m_pMeshCom->SetUp_AnimationSet(11);
		switch (m_pPlayer->Get_PlayerWeapon())
		{
		case WEAPON_PISTOL:
			m_iCurHp -= 7;
			break;
		case WEAPON_RIFFLE:
			m_iCurHp -= 9;
			break;
		case WEAPON_SNIPER:
			m_iCurHp -= 13;
			break;
		}
		m_bIsHit = true;
		pManagement->Add_GameObjectToLayer(L"GameObject_Bleeding", SCENE_STAGE, L"Layer_Effect", &(_vec3)m_matRealMtx[HITBOX_LEFTARM].m[3]);

		return true;
	}

	else if (m_pColliderCom[HITBOX_RIGHTARM]->Collision_Sphere(pCollider))
	{
		m_pMeshCom->SetUp_AnimationSet(13);
		switch (m_pPlayer->Get_PlayerWeapon())
		{
		case WEAPON_PISTOL:
			m_iCurHp -= 7;
			break;
		case WEAPON_RIFFLE:
			m_iCurHp -= 9;
			break;
		case WEAPON_SNIPER:
			m_iCurHp -= 13;
			break;
		}
		m_bIsHit = true;
		pManagement->Add_GameObjectToLayer(L"GameObject_Bleeding", SCENE_STAGE, L"Layer_Effect", &(_vec3)m_matRealMtx[HITBOX_RIGHTARM].m[3]);

		return true;
	}

	else if (m_pColliderCom[HITBOX_LEFTCLAFT]->Collision_Sphere(pCollider))
	{
		m_pMeshCom->SetUp_AnimationSet(10);
		switch (m_pPlayer->Get_PlayerWeapon())
		{
		case WEAPON_PISTOL:
			m_iCurHp -= 7;
			break;
		case WEAPON_RIFFLE:
			m_iCurHp -= 9;
			break;
		case WEAPON_SNIPER:
			m_iCurHp -= 13;
			break;
		}
		m_bIsHit = true;
		pManagement->Add_GameObjectToLayer(L"GameObject_Bleeding", SCENE_STAGE, L"Layer_Effect", &(_vec3)m_matRealMtx[HITBOX_LEFTCLAFT].m[3]);

		return true;
	}

	else if (m_pColliderCom[HITBOX_RIGHTCLAFT]->Collision_Sphere(pCollider))
	{
		m_pMeshCom->SetUp_AnimationSet(12);
		switch (m_pPlayer->Get_PlayerWeapon())
		{
		case WEAPON_PISTOL:
			m_iCurHp -= 7;
			break;
		case WEAPON_RIFFLE:
			m_iCurHp -= 9;
			break;
		case WEAPON_SNIPER:
			m_iCurHp -= 13;
			break;
		}
		m_bIsHit = true;
		pManagement->Add_GameObjectToLayer(L"GameObject_Bleeding", SCENE_STAGE, L"Layer_Effect", &(_vec3)m_matRealMtx[HITBOX_RIGHTCLAFT].m[3]);
		return true;
	}

	return false;
}

void CMonster::UpdateCollider()
{
	m_matRealMtx[HITBOX_HEAD] = *m_matHead * *m_pTransformCom->Get_WorldMatrixPointer();
	m_matRealMtx[HITBOX_BODY] = *m_matBody * *m_pTransformCom->Get_WorldMatrixPointer();
	m_matRealMtx[HITBOX_LEFTCLAFT] = *m_matLeftCalf * *m_pTransformCom->Get_WorldMatrixPointer();
	m_matRealMtx[HITBOX_RIGHTCLAFT] = *m_matRightCalf * *m_pTransformCom->Get_WorldMatrixPointer();
	m_matRealMtx[HITBOX_LEFTARM] = *m_matLeftArm * *m_pTransformCom->Get_WorldMatrixPointer();
	m_matRealMtx[HITBOX_RIGHTARM] = *m_matRightArm * *m_pTransformCom->Get_WorldMatrixPointer();
}

void CMonster::Cheat()
{
	m_iCurHp -= 100;
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonster* pInstance = new CMonster(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster::Clone_GameObject(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CMonster Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	for (int i = 0; i < HITBOX_END; i++)
	{
		Safe_Release(m_pColliderCom[i]);
	}
	Safe_Release(m_pPlayer);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pMeshCom);

	CGameObject::Free();
}
