#include "stdafx.h"
#include "..\Headers\Player.h"
#include "Management.h"

#include "Sniper.h"
#include "Pistol.h"
#include "Riffle.h"

#include "Camera_Free.h"
#include "Bullet.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice),
	m_pPipeLineCom(CPipeLine::GetInstance())
{
	Safe_AddRef(m_pPipeLineCom);
}

CPlayer::CPlayer(const CPlayer & rhs)
	:CGameObject(rhs),
	m_pPipeLineCom(rhs.m_pPipeLineCom)
{
	Safe_AddRef(m_pPipeLineCom);
}

HRESULT CPlayer::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(10.f, 10.f, 10.f));

	m_eType = WEAPON_PISTOL;
	m_ePreType = m_eType;

	m_fCoolTime = 0.f;

	m_bIsDead = false;
	
	return S_OK;
}

_int CPlayer::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;
	CManagement* pManagement = CManagement::GetInstance();
	Safe_AddRef(pManagement);

	GetWeaponCurrentBullet();

	ComputeRay();

	m_fCoolTime -= 1.f * (float)TimeDelta;

	if (nullptr == pManagement)
		return -1;
	
	PlayerMove(m_TimeDelta);

	if (pManagement->KeyDown(KEY_1))
	{
		m_eType = WEAPON_PISTOL;
		if (m_ePreType != m_eType)
		{
			m_pMeshCom->SetUp_AnimationSet(3);
			m_eState = STATE_SWAP;
		}
	}

	else if (pManagement->KeyDown(KEY_2))
	{
		m_eType = WEAPON_RIFFLE;
		if (m_ePreType != m_eType)
		{
			m_pMeshCom->SetUp_AnimationSet(13);
			m_eState = STATE_SWAP;
		}
	}

	else if (pManagement->KeyDown(KEY_3))
	{
		m_eType = WEAPON_SNIPER;
		if (m_ePreType != m_eType)
		{
			m_pMeshCom->SetUp_AnimationSet(8);
			m_eState = STATE_SWAP;
		}
	}

	if (m_iCurHp <= 0)
		m_iCurHp = 1;

	_long Mouse = 0;

	if (Mouse = pManagement->Get_DIMMove(CInput::DIM_X))
		m_pTransformCom->Rotation_Axis(&_vec3(0.f, 1.f, 0.f), TimeDelta * (Mouse * 0.2f));
	
	if (Mouse = pManagement->Get_DIMMove(CInput::DIM_Y))
	{
		if (m_pTransformCom->Get_State(CTransform::STATE_UP).y >= 0.85f)
			m_pTransformCom->Rotation_Axis(&m_pTransformCom->Get_State(CTransform::STATE_RIGHT), TimeDelta * (Mouse * 0.2f));

		else
		{
			_vec3 TempVec = m_pTransformCom->Get_State(CTransform::STATE_UP);
			m_pTransformCom->Set_State(CTransform::STATE_UP, _vec3(TempVec.x,0.85f,TempVec.z));
		}
	}

	SetAnimationTIme();

	SetUp_OnTerrain();

	m_pColliderCom->Update_Collider(TimeDelta);

	if (true == m_pMeshCom->is_Finished(m_dAnimationTime))
	{
		switch (m_eType)
		{
		case WEAPON_PISTOL:
			m_pMeshCom->SetUp_AnimationSet(4);
			break;
		case WEAPON_RIFFLE:
			m_pMeshCom->SetUp_AnimationSet(14);
			break;
		case WEAPON_SNIPER:
			m_pMeshCom->SetUp_AnimationSet(9);
			break;
		}
		m_eState = STATE_IDLE;
	}

	if (m_bIsAttacked)
	{
		if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_HitScreen", SCENE_STAGE, L"Layer_UI")))
			return E_FAIL;
		m_bIsAttacked = false;
	}

	if (pManagement->KeyDown(KEY_UP))
		m_bIsShow = !m_bIsShow;

	m_ePreType = m_eType;
	Safe_Release(pManagement);
	return _int();
}

_int CPlayer::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_vec3 CPlayer::Get_PlayerPos() const
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION);	
}

_vec3 CPlayer::Get_Player_HeadPos() const
{
	return m_vPos;
}

_vec3 CPlayer::Get_Ray() const
{
	if (GetKeyState(VK_LBUTTON) & 0x8000 && m_eState != STATE_RELOAD)
	{
		return m_vRay;
	}

	return _vec3();
}

HRESULT CPlayer::Render_GameObject()
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

			m_pShaderCom->Commit_Change();

			m_pMeshCom->Render_Mesh(i, j);
		}
	}

	m_pShaderCom->End_Pass();
	m_pShaderCom->End_Shader();
	
#ifdef _DEBUG
	if (m_bIsShow)
	{
		if (FAILED(m_pColliderCom->Render_Collider()))
			return E_FAIL;
	}
#endif

	return S_OK;
}

void CPlayer::PlayerMove(_double TimeDelta)
{
	CManagement* pManagement = CManagement::GetInstance();
	Safe_AddRef(pManagement);

	if (pManagement->KeyPressing(KEY_W))
	{
		m_pTransformCom->Go_Straight(m_pNavigationCom, TimeDelta);

		switch (m_eType)
		{
		case WEAPON_PISTOL:
			m_pMeshCom->SetUp_AnimationSet(1);
			break;
		case WEAPON_RIFFLE:
			m_pMeshCom->SetUp_AnimationSet(11);
			break;
		case WEAPON_SNIPER:
			m_pMeshCom->SetUp_AnimationSet(6);
			break;
		}
		m_eState = STATE_RUN;
	}

	else if (pManagement->KeyPressing(KEY_S))
	{
		m_pTransformCom->Go_Backward(m_pNavigationCom, TimeDelta);

		switch (m_eType)
		{
		case WEAPON_PISTOL:
			m_pMeshCom->SetUp_AnimationSet(1);
			break;
		case WEAPON_RIFFLE:
			m_pMeshCom->SetUp_AnimationSet(11);
			break;
		case WEAPON_SNIPER:
			m_pMeshCom->SetUp_AnimationSet(6);
			break;
		}
		m_eState = STATE_RUN;
	}

	if (pManagement->KeyPressing(KEY_A))
	{
		m_pTransformCom->Go_Left(m_pNavigationCom, TimeDelta);

		switch (m_eType)
		{
		case WEAPON_PISTOL:
			m_pMeshCom->SetUp_AnimationSet(1);
			break;
		case WEAPON_RIFFLE:
			m_pMeshCom->SetUp_AnimationSet(11);
			break;
		case WEAPON_SNIPER:
			m_pMeshCom->SetUp_AnimationSet(6);
			break;
		}
		m_eState = STATE_RUN;
	}

	else if (pManagement->KeyPressing(KEY_D))
	{
		m_pTransformCom->Go_Right(m_pNavigationCom, TimeDelta);

		switch (m_eType)
		{
		case WEAPON_PISTOL:
			m_pMeshCom->SetUp_AnimationSet(1);
			break;
		case WEAPON_RIFFLE:
			m_pMeshCom->SetUp_AnimationSet(11);
			break;
		case WEAPON_SNIPER:
			m_pMeshCom->SetUp_AnimationSet(6);
			break;
		}
		m_eState = STATE_RUN;
	}

	if (pManagement->KeyUp(KEY_R))
	{
		m_eState = STATE_RELOAD;
		switch (m_eType)
		{
		case WEAPON_PISTOL:
			m_pMeshCom->SetUp_AnimationSet(2);
			break;
		case WEAPON_RIFFLE:
			m_pMeshCom->SetUp_AnimationSet(12);
			break;
		case WEAPON_SNIPER:
			m_pMeshCom->SetUp_AnimationSet(7);
			break;
		}
	}

	else if (pManagement->KeyPressing(KEY_LBUTTON) && m_iBulletResiduum > 0 && m_fCoolTime <= 0.f)
	{
		switch (m_eType)
		{
		case WEAPON_PISTOL:
			m_pMeshCom->SetUp_AnimationSet(0);
			m_eState = STATE_ATTACK;
			if (m_iBulletResiduum <= 0)
			{
				m_eState = STATE_RELOAD;
				m_pMeshCom->SetUp_AnimationSet(2);
			}
			m_fCoolTime = 0.5f;
			break;
		case WEAPON_RIFFLE:
			m_pMeshCom->SetUp_AnimationSet(10);
			m_eState = STATE_ATTACK;
			if (m_iBulletResiduum <= 0)
			{
				m_eState = STATE_RELOAD;
				m_pMeshCom->SetUp_AnimationSet(12);
			}
			m_fCoolTime = 0.35f;
			break;
		case WEAPON_SNIPER:
			m_pMeshCom->SetUp_AnimationSet(5);
			m_eState = STATE_ATTACK;
			if (m_iBulletResiduum <= 0)
			{
				m_eState = STATE_RELOAD;
				m_pMeshCom->SetUp_AnimationSet(7);
			}
			m_fCoolTime = 1.5f;
			break;
		}
		pManagement->Add_GameObjectToLayer(L"GameObject_Bullet", SCENE_STAGE, L"Layer_Bullet", m_vRay);

		//pManagement->PlaySound(L"Riffle_Fire.mp3",CSound::EFFECT);

		D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameDerived("Weapon");
		_matrix* pHandMatrix = &pFrame->CombinedTransformationMatrix;
		_vec3 TempVec = (_vec3)pHandMatrix->m[3];
		switch (m_eType)
		{
		case WEAPON_PISTOL:
			D3DXVec3TransformCoord(&m_vMuzzle, &_vec3(TempVec.x, TempVec.y, TempVec.z + 0.5f), &m_pTransformCom->Get_WorldMatrix());
			break;
		case WEAPON_RIFFLE:
			D3DXVec3TransformCoord(&m_vMuzzle, &_vec3(TempVec.x, TempVec.y, TempVec.z + 1.2f), &m_pTransformCom->Get_WorldMatrix());
			break;
		case WEAPON_SNIPER:
			D3DXVec3TransformCoord(&m_vMuzzle, &_vec3(TempVec.x, TempVec.y, TempVec.z + 1.5f), &m_pTransformCom->Get_WorldMatrix());
			break;
		}


		D3DLIGHT9 PointLight;
		ZeroMemory(&PointLight, sizeof(D3DLIGHT9));

		PointLight.Type = D3DLIGHT_POINT;
		PointLight.Range = 0.5f;
		PointLight.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		PointLight.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
		PointLight.Specular = PointLight.Diffuse;

		PointLight.Position = m_vMuzzle;

		pManagement->Add_Light(Get_Graphic_Device(), &PointLight, 0.5);
	}

	Safe_Release(pManagement);
}

void CPlayer::SetAnimationTIme()
{
	switch (m_eState)
	{
	case STATE_SWAP:
		m_dAnimationTime = -0.3;
		break;
	case STATE_RUN:
		m_dAnimationTime = -0.0015;
		break;
	case STATE_ATTACK:
		m_dAnimationTime = 0;
		break;
	case STATE_RELOAD:
		m_dAnimationTime = 0;
		break;
	case STATE_IDLE:
		m_dAnimationTime = 0;
		break;
	}
}

void CPlayer::GetWeaponCurrentBullet()
{
	CManagement* pManagement = CManagement::GetInstance();
	Safe_AddRef(pManagement);

	if (nullptr == pManagement)
		return; 
	CSniper* pSniper = dynamic_cast<CSniper*>(pManagement->Get_GameObject(L"GameObject_Sniper", SCENE_STAGE, L"Layer_Player", 3));
	Safe_AddRef(pSniper);

	CRiffle* pRiffle = dynamic_cast<CRiffle*>(pManagement->Get_GameObject(L"GameObject_Riffle", SCENE_STAGE, L"Layer_Player", 2));
	Safe_AddRef(pRiffle);

	CPistol* pPistol = dynamic_cast<CPistol*>(pManagement->Get_GameObject(L"GameObject_Pistol", SCENE_STAGE, L"Layer_Player", 1));

	Safe_AddRef(pPistol);

	switch (m_eType)
	{
	case WEAPON_SNIPER:
		if (nullptr != pSniper)
			m_iBulletResiduum = pSniper->Get_CurBullet();

		break;

	case WEAPON_RIFFLE:
		if (nullptr != pRiffle)
			m_iBulletResiduum = pRiffle->Get_CurBullet();

		break;

	case WEAPON_PISTOL:
		if (nullptr != pPistol)
			m_iBulletResiduum = pPistol->Get_CurBullet();

		break;
	}

	Safe_Release(pSniper);
	Safe_Release(pRiffle);
	Safe_Release(pPistol);
	Safe_Release(pManagement);
}

bool CPlayer::IsCanFire()
{
	if(m_iBulletResiduum > 0)
	return true;

	return false;
}

void CPlayer::ComputeRay()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return;

	Safe_AddRef(pManagement);


	D3DDEVICE_CREATION_PARAMETERS			Parameters;
	Get_Graphic_Device()->GetCreationParameters(&Parameters);

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(Parameters.hFocusWindow, &ptMouse);

	// 투영스페이스 상의 마우스 좌표를 구하자.
	D3DVIEWPORT9		ViewPort;
	Get_Graphic_Device()->GetViewport(&ViewPort);

	_vec4		vMousePoint;

	vMousePoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePoint.z = 0.f;
	vMousePoint.w = 1.f;

	// 뷰스페이스 상의 레이와 피벗을 구하자.
	_matrix		matProj = m_pPipeLineCom->Get_Transform(D3DTS_PROJECTION);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	D3DXVec4Transform(&vMousePoint, &vMousePoint, &matProj);

	_vec3		vRay, vPivot;

	vPivot = _vec3(0.f, 0.f, 0.f);
	vRay = _vec3(vMousePoint.x, 0.f, vMousePoint.z) - vPivot;

	// 월드 스페이스 상의 레이와 피벗을 구하자.
	_matrix		matView = m_pPipeLineCom->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	D3DXVec3TransformNormal(&vRay, &vRay, &matView);
	D3DXVec3TransformCoord(&vPivot, &vPivot, &matView);

	m_vRay = vRay;
	m_vPos = vPivot;

	Safe_Release(pManagement);

}

HRESULT CPlayer::Add_Component()
{
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Mesh_Player", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;


	// For.Com_Transform
	CTransform::STATEDESC		StateDesc;
	StateDesc.fSpeedPerSec = 5.f;
	StateDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &StateDesc)))
		return E_FAIL;


	// For.Com_Collider_AABB
	CCollider::COLLIDER_DESC	Collider_Desc;
	Collider_Desc.vLocalScale = _vec3(1.f, 2.f, 1.f);
	Collider_Desc.vLocalPosition = _vec3(0.f, 1.f, 0.f);
	Collider_Desc.pTargetMatrix = m_pTransformCom->Get_WorldMatrixPointer();

	// For.Com_Collider
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider", L"Com_Collider", (CComponent**)&m_pColliderCom, &Collider_Desc)))
		return E_FAIL;

	// For.Com_Navigation
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Stage_Navigation", L"Com_Navigation", (CComponent**)&m_pNavigationCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::SetUp_ConstantTable()
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

HRESULT CPlayer::SetUp_OnTerrain()
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

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer* pInstance = new CPlayer(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone_GameObject(void * pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	Safe_Release(m_pPipeLineCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pNavigationCom);

	CGameObject::Free();
}
