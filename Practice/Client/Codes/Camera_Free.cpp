#include "stdafx.h"
#include "..\Headers\Camera_Free.h"
#include "Management.h"

#include "Player.h"


CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pDevice)
	:CCamera(pDevice)
	,m_bIsReady(false)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs)
	:CCamera(rhs)
	, m_bIsReady(rhs.m_bIsReady)
{
}

HRESULT CCamera_Free::Ready_GameObject_Prototype()
{


	return S_OK;
}

HRESULT CCamera_Free::Ready_GameObject(void * pArg)
{
	if (FAILED(CCamera::Ready_GameObject(pArg)))
		return E_FAIL; 
	
	return S_OK;
}

HRESULT CCamera_Free::Late_Ready_GameObject()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pTransform = (CTransform*)pManagement->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Transform");
	if (nullptr == pTransform)
		return E_FAIL; 

	CMesh_Dynamic* pPlayerMeshCom = (CMesh_Dynamic*)pManagement->Get_Component(SCENE_STAGE, L"Layer_Player", L"Com_Mesh");
	if (nullptr == pPlayerMeshCom)
		return E_FAIL;

	D3DXFRAME_DERIVED*	pFrame = pPlayerMeshCom->Get_FrameDerived("Dummy21");
	m_pHeadMatrix = &pFrame->CombinedTransformationMatrix;

	Safe_AddRef(pTransform);

	m_pPlayerMatrix = pTransform->Get_WorldMatrixPointer();

	Safe_Release(pTransform);

	return S_OK;
}

_int CCamera_Free::Update_GameObject(_double TimeDelta)
{
	if (!m_bIsReady)
	{
		Late_Ready_GameObject();
		m_bIsReady = true;
	}

	POINT pt = {g_WINCX >> 1, g_WINCY >> 1};

	ClientToScreen(g_hWnd, &pt);

	SetCursorPos(pt.x, pt.y);


	return CCamera::Update_GameObject(TimeDelta);
}

_int CCamera_Free::LateUpdate_GameObject(_double TimeDelta)
{

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	_matrix tempMat = *m_pHeadMatrix * *m_pPlayerMatrix;
	
	D3DXVec3Normalize((_vec3*)&m_pPlayerMatrix->m[0], (_vec3*)&m_pPlayerMatrix->m[0]);
	D3DXVec3Normalize((_vec3*)&m_pPlayerMatrix->m[1], (_vec3*)&m_pPlayerMatrix->m[1]);
	D3DXVec3Normalize((_vec3*)&m_pPlayerMatrix->m[2], (_vec3*)&m_pPlayerMatrix->m[2]);

	// 플레이어 매트릭스에 카메라를 고정
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_vec3*)&m_pPlayerMatrix->m[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_vec3*)&m_pPlayerMatrix->m[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_vec3*)&m_pPlayerMatrix->m[2]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, tempMat.m[3]);

	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, _vec3(1.f,0.f,0.f));
	//m_pTransformCom->Set_State(CTransform::STATE_UP, _vec3(0.f, 1.f, 0.f));
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, _vec3(0.f, 0.f, 1.f));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(10.f, 10.f, 10.f));

	Safe_Release(pManagement);

	return _int();
}

HRESULT CCamera_Free::Render_GameObject()
{
	return S_OK;
}

HRESULT CCamera_Free::Camera_Shaking()
{
	if (m_bIsDead || m_pTransformCom == nullptr)
		return E_FAIL;

	for (int i = 0; i < 20; i++)
	{
		m_pTransformCom->Set_Rotation((_vec3*)&m_pPlayerMatrix->m[0], 3.f);
	}


	return S_OK;
}

CCamera_Free * CCamera_Free::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Free::Clone_GameObject(void * pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CCamera_Free_Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	Safe_Release(m_pTransformCom);

	CCamera::Free();
}
