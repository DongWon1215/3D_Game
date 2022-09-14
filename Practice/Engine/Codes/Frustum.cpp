#include "..\Headers\Frustum.h"

CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pPipeLine(CPipeLine::GetInstance())
{
	Safe_AddRef(m_pPipeLine);

}
HRESULT CFrustum::Ready_Component_Prototype()
{
	// 원형객체 생성 시, 투영스페이스 상의 절두체를 구성.
	m_vProjPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vProjPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vProjPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vProjPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_vProjPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vProjPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vProjPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vProjPoint[7] = _vec3(-1.f, -1.f, 1.f);

	return NOERROR;
}

HRESULT CFrustum::Ready_Component(void * pArg)
{


	return NOERROR;
}

_bool CFrustum::Culling_ToFrustum(CTransform * pTransform, _float fRadius)
{
	Transform_ToLocal(pTransform);

	_vec3		vLocalPos;

	_matrix		WorldMatrix = pTransform->Get_WorldMatrixInverse();

	D3DXVec3TransformCoord(&vLocalPos, &pTransform->Get_State(CTransform::STATE_POSITION), &WorldMatrix);

	return !is_InFrustum(vLocalPos, fRadius);
}

HRESULT CFrustum::Transform_ToLocal(CTransform * pTransform)
{
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	_vec3		vPoint[8];

	_matrix		ProjMatrix = m_pPipeLine->Get_Transform(D3DTS_PROJECTION);
	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);

	_matrix		ViewMatrix = m_pPipeLine->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_matrix		WorldMatrix = pTransform->Get_WorldMatrixInverse();

	for (size_t i = 0; i < 8; ++i)
	{
		// 뷰스페이스 상의 절두체 여덟개점을 구한다.
		D3DXVec3TransformCoord(&vPoint[i], &m_vProjPoint[i], &ProjMatrix);

		// 월드스페이스 상의 절두체 여덟개점을 구한다.
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &ViewMatrix);

		// 로컬스페이스 상의 절두체 여덟개점을 구한다.
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &WorldMatrix);
	}

	// +x
	D3DXPlaneFromPoints(&m_Plane[0], &vPoint[1], &vPoint[5], &vPoint[6]);
	// -x
	D3DXPlaneFromPoints(&m_Plane[1], &vPoint[4], &vPoint[0], &vPoint[3]);

	// +y
	D3DXPlaneFromPoints(&m_Plane[2], &vPoint[4], &vPoint[5], &vPoint[1]);
	// -y
	D3DXPlaneFromPoints(&m_Plane[3], &vPoint[3], &vPoint[2], &vPoint[6]);

	// +z
	D3DXPlaneFromPoints(&m_Plane[4], &vPoint[5], &vPoint[4], &vPoint[7]);
	// -z
	D3DXPlaneFromPoints(&m_Plane[5], &vPoint[0], &vPoint[1], &vPoint[2]);

	return NOERROR;
}

_bool CFrustum::is_InFrustum(_vec3 vPointInLocal, _float fRadius)
{
	// ax + by + cz + d > 0

	// a b c d
	// x y z 1

	for (size_t i = 0; i < 6; ++i)
	{
		if (D3DXPlaneDotCoord(&m_Plane[i], &vPointInLocal) > fRadius)
			return false;
	}


	return _bool(true);
}

CFrustum * CFrustum::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFrustum* pInstance = new CFrustum(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MSG_BOX("Failed while Creating CFrustum");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CFrustum::Clone_Component(void * pArg)
{
	AddRef();

	return this;
}


void CFrustum::Free()
{
	Safe_Release(m_pPipeLine);

	CComponent::Free();
}
