#include "..\Headers\Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pPipeLine(CPipeLine::GetInstance())
{
	Safe_AddRef(m_pPipeLine);

}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, m_pPipeLine(rhs.m_pPipeLine)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CCamera::Ready_GameObject_Prototype()
{


	return NOERROR;
}

HRESULT CCamera::Ready_GameObject(void * pArg)
{
	m_pTransformCom = CTransform::Create(Get_Graphic_Device());
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CAMERADESC		CameraDesc = *(CAMERADESC*)pArg;

	CTransform::STATEDESC	StateDesc;

	StateDesc.fSpeedPerSec = CameraDesc.fSpeedPerSec;
	StateDesc.fRotationPerSec = CameraDesc.fRotationPerSec;

	m_pTransformCom->Set_StateDesc(StateDesc);


	_vec3		vRight, vUp, vLook, vPosition;

	vPosition = CameraDesc.vEye;

	vLook = CameraDesc.vAt - vPosition;
	D3DXVec3Normalize(&vLook, &vLook);

	D3DXVec3Cross(&vRight, &CameraDesc.vAxisY, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	m_ViewMatrix = m_pTransformCom->Get_WorldMatrixInverse();

	D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, CameraDesc.fFovy, CameraDesc.fAspect, CameraDesc.fNear, CameraDesc.fFar);

	return NOERROR;
}

_int CCamera::Update_GameObject(_double TimeDelta)
{
	m_pPipeLine->Set_Transform(D3DTS_VIEW, m_ViewMatrix = m_pTransformCom->Get_WorldMatrixInverse());
	m_pPipeLine->Set_Transform(D3DTS_PROJECTION, m_ProjMatrix);

	return _int();
}

_int CCamera::LateUpdate_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CCamera::Render_GameObject()
{
	return NOERROR;
}

void CCamera::Free()
{
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
