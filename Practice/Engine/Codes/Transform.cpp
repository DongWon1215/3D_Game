#include "..\Headers\Transform.h"
#include "..\Headers\Navigation.h"


CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

_matrix CTransform::Get_WorldMatrixInverse() const
{
	_matrix		matWorldInv;

	D3DXMatrixInverse(&matWorldInv, nullptr, &m_WorldMatrix);

	return _matrix(matWorldInv);
}

void CTransform::Set_StateDesc(STATEDESC StateDesc)
{
	m_StateDesc = StateDesc;
}

void CTransform::Set_Rotation(const _vec3* pAxis, _float fRadian)
{
	_matrix		matRotation;

	D3DXMatrixRotationAxis(&matRotation, pAxis, fRadian);

	_vec3		vRight, vUp, vLook;

	vRight = _vec3(1.f, 0.f, 0.f) * Get_Scale(CTransform::STATE_RIGHT);
	vUp = _vec3(0.f, 1.f, 0.f) * Get_Scale(CTransform::STATE_UP);
	vLook = _vec3(0.f, 0.f, 1.f) * Get_Scale(CTransform::STATE_LOOK);

	D3DXVec3TransformNormal((_vec3*)&m_WorldMatrix.m[STATE_RIGHT][0], &vRight, &matRotation);
	D3DXVec3TransformNormal((_vec3*)&m_WorldMatrix.m[STATE_UP][0], &vUp, &matRotation);
	D3DXVec3TransformNormal((_vec3*)&m_WorldMatrix.m[STATE_LOOK][0], &vLook, &matRotation);


}

void CTransform::Set_Scale(_vec3 vScale) // 내가 지정한 사이즈로 변형한다.
{
	_vec3		vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp = Get_State(CTransform::STATE_UP);
	vLook = Get_State(CTransform::STATE_LOOK);

	vRight = *D3DXVec3Normalize(&vRight, &vRight) * vScale.x;
	vUp = *D3DXVec3Normalize(&vUp, &vUp) * vScale.y;
	vLook = *D3DXVec3Normalize(&vLook, &vLook) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

HRESULT CTransform::Ready_Component_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return NOERROR;
}

HRESULT CTransform::Ready_Component(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_StateDesc, pArg, sizeof(STATEDESC));

	return NOERROR;
}

HRESULT CTransform::Go_Straight(_double TimeDelta)
{
	_vec3	vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3	vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_StateDesc.fSpeedPerSec * _float(TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Go_Backward(_double TimeDelta)
{
	_vec3	vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3	vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_StateDesc.fSpeedPerSec * _float(TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Go_Left(_double TimeDelta)
{
	_vec3	vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3	vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_StateDesc.fSpeedPerSec * _float(TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Go_Right(_double TimeDelta)
{
	_vec3	vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3	vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_StateDesc.fSpeedPerSec * _float(TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Rotation_Axis(const _vec3 * pAxis, _double TimeDelta)
{
	_matrix		matRotation;

	D3DXMatrixRotationAxis(&matRotation, pAxis, m_StateDesc.fRotationPerSec * TimeDelta);

	_vec3		vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp = Get_State(CTransform::STATE_UP);
	vLook = Get_State(CTransform::STATE_LOOK);

	D3DXVec3TransformNormal((_vec3*)&m_WorldMatrix.m[STATE_RIGHT][0], &vRight, &matRotation);
	D3DXVec3TransformNormal((_vec3*)&m_WorldMatrix.m[STATE_UP][0], &vUp, &matRotation);
	D3DXVec3TransformNormal((_vec3*)&m_WorldMatrix.m[STATE_LOOK][0], &vLook, &matRotation);

	return NOERROR;
}

_float CTransform::Go_Target(_vec3 vTargetPos, _double TimeDelta)
{
	_vec3		vPosition = Get_State(CTransform::STATE_POSITION);

	_vec3		vDir = vTargetPos - vPosition;

	_float		fDistance = D3DXVec3Length(&vDir);

	_vec3		vRight, vUp, vLook;

	vUp = Get_State(CTransform::STATE_UP);

	D3DXVec3Cross(&vRight, &vUp, &vDir);

	D3DXVec3Cross(&vLook, &vRight, &vUp);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * Get_Scale(CTransform::STATE_RIGHT));
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * Get_Scale(CTransform::STATE_LOOK));

	Go_Straight(TimeDelta);

	return fDistance;
}

// for. Navimesh
HRESULT CTransform::Go_Straight(CNavigation * pNavigation, _double TimeDelta)
{
	_vec3	vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3	vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_StateDesc.fSpeedPerSec * _float(TimeDelta);

	if (true == pNavigation->is_Move(&vPosition))
		Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Go_Backward(CNavigation * pNavigation, _double TimeDelta)
{
	_vec3	vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3	vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_StateDesc.fSpeedPerSec * _float(TimeDelta);

	if (true == pNavigation->is_Move(&vPosition))
		Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Go_Left(CNavigation * pNavigation, _double TimeDelta)
{
	_vec3	vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3	vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_StateDesc.fSpeedPerSec * _float(TimeDelta);

	if (true == pNavigation->is_Move(&vPosition))
		Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Go_Right(CNavigation * pNavigation, _double TimeDelta)
{
	_vec3	vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3	vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_StateDesc.fSpeedPerSec * _float(TimeDelta);

	if (true == pNavigation->is_Move(&vPosition))
		Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

_float CTransform::Go_Target(CNavigation * pNavigation, _vec3 vTargetPos, _double TimeDelta)
{
	_vec3		vPosition = Get_State(CTransform::STATE_POSITION);

	_vec3		vDir = vTargetPos - vPosition;

	_float		fDistance = D3DXVec3Length(&vDir);

	_vec3		vRight, vUp, vLook;

	vUp = Get_State(CTransform::STATE_UP);

	D3DXVec3Cross(&vRight, &vUp, &vDir);

	D3DXVec3Cross(&vLook, &vRight, &vUp);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * Get_Scale(CTransform::STATE_RIGHT));
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * Get_Scale(CTransform::STATE_LOOK));

	if (true == pNavigation->is_Move(&vPosition))
		Go_Straight(TimeDelta);

	return fDistance;
}

HRESULT CTransform::Remove_Scale()
{
	D3DXVec3Normalize((_vec3*)&m_WorldMatrix.m[0][0], (_vec3*)&m_WorldMatrix.m[0][0]);
	D3DXVec3Normalize((_vec3*)&m_WorldMatrix.m[1][0], (_vec3*)&m_WorldMatrix.m[1][0]);
	D3DXVec3Normalize((_vec3*)&m_WorldMatrix.m[2][0], (_vec3*)&m_WorldMatrix.m[2][0]);

	return S_OK;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform* pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MSG_BOX("Failed while Creating CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone_Component(void * pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed while Creating CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{

	CComponent::Free();
}
