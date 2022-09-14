#include "stdafx.h"
#include "../Headers/Riffle.h"
#include "Management.h"

#include "Player.h"

CRiffle::CRiffle(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CRiffle::CRiffle(const CRiffle & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRiffle::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CRiffle::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_bIsDead = false;

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	CMesh_Dynamic* pPlayerMeshCom = (CMesh_Dynamic*)pManagement->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Mesh");
	if (nullptr == pPlayerMeshCom)
		return E_FAIL;

	Safe_AddRef(pPlayerMeshCom);

	D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameDerived("Weapon");
	m_pHandMatrix = &pFrame->CombinedTransformationMatrix;

	Safe_Release(pPlayerMeshCom);

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	Safe_AddRef(pPlayerTransform);

	m_pPlayerWorldMatrix = pPlayerTransform->Get_WorldMatrixPointer();

	Safe_Release(pPlayerTransform);

	Safe_Release(pManagement);

	m_iMaxBullet = 30.f;
	m_iCurrentBullet = m_iMaxBullet;

	return S_OK;
}

_int CRiffle::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;
	CManagement* pManagement = CManagement::GetInstance();
	Safe_AddRef(pManagement);

	if (nullptr == pManagement)
		return -1;

	m_ePlayerState = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(L"GameObject_Player", SCENE_STAGE, L"Layer_Player", 0))->Get_PlayerState();
	m_eWeapon = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(L"GameObject_Player", SCENE_STAGE, L"Layer_Player", 0))->Get_PlayerWeapon();

	if (m_pMeshCom->is_Finished(TimeDelta) && m_bIsFrameStart)
	{
		if(m_ePlayerState != STATE_ATTACK)
		m_pMeshCom->SetUp_AnimationSet(0);

		else
			m_pMeshCom->SetUp_AnimationSet(2);

		m_bIsFrameStart = false;
	}

	m_fCoolTime -= 1.f * (float)TimeDelta;

	Safe_Release(pManagement);
	return _int();
}

_int CRiffle::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_eWeapon == WEAPON_RIFFLE)
	{
		if (nullptr == m_pRendererCom)
			return -1;

		CManagement* pManagement = CManagement::GetInstance();
		if (nullptr == pManagement)
			return -1;

		Safe_AddRef(pManagement);

		if (pManagement->KeyPressing(KEY_LBUTTON) && m_iCurrentBullet > 0 && !m_bIsFrameStart && m_fCoolTime <= 0.f)
		{
			m_pMeshCom->SetUp_AnimationSet(0);
			m_bIsFrameStart = true;
			m_iCurrentBullet --;
			m_fCoolTime = 0.35f;
		}
		
		else if (dynamic_cast<CPlayer*>(pManagement->Get_GameObject(L"GameObject_Player", SCENE_STAGE, L"Layer_Player", 0))->Get_PlayerState() == STATE_RELOAD && !m_bIsFrameStart || m_iCurrentBullet <= 0)
		{
			m_pMeshCom->SetUp_AnimationSet(1);
			m_bIsFrameStart = true;
			m_iCurrentBullet = m_iMaxBullet;
		}
		
		else if (m_ePlayerState == STATE_SWAP && !m_bIsFrameStart)
		{
			m_pMeshCom->SetUp_AnimationSet(2);
		m_bIsFrameStart = true;
		}

		_matrix TempMatrix = *m_pHandMatrix * *m_pPlayerWorldMatrix;
		D3DXVec3Normalize((_vec3*)TempMatrix.m[0], (_vec3*)TempMatrix.m[0]);
		D3DXVec3Normalize((_vec3*)TempMatrix.m[1], (_vec3*)TempMatrix.m[1]);
		D3DXVec3Normalize((_vec3*)TempMatrix.m[2], (_vec3*)TempMatrix.m[2]);


		m_RenderingMatrix = m_pTransformCom->Get_WorldMatrix() * TempMatrix;
		
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

		Safe_Release(pManagement);
	}
	
	return _int();
}

HRESULT CRiffle::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pMeshCom)
		return E_FAIL;

	if(m_bIsFrameStart)
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

			m_pShaderCom->Commit_Change();

			m_pMeshCom->Render_Mesh(i, j);
		}
	}

	m_pShaderCom->End_Pass();
	m_pShaderCom->End_Shader();

	return S_OK;
}

HRESULT CRiffle::Add_Component()
{
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Mesh_Riffle", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Transform
	CTransform::STATEDESC		StateDesc;
	StateDesc.fSpeedPerSec = 5.f;
	//StateDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &StateDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRiffle::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom)
		return E_FAIL;

	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(m_pShaderCom->Set_Value("g_matWorld", &m_RenderingMatrix, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Value("g_matView", &pManagement->Get_Transform(D3DTS_VIEW), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Value("g_matProj", &pManagement->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix))))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

CRiffle * CRiffle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRiffle* pInstance = new CRiffle(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CRiffle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRiffle::Clone_GameObject(void * pArg)
{
	CRiffle* pInstance = new CRiffle(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CRiffle_Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRiffle::Free()
{
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
