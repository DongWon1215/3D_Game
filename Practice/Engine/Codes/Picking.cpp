#include "..\Headers\Picking.h"
#include "..\Headers\Transform.h"
#include "..\Headers\PipeLine.h"

CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pDevice(pGraphic_Device)
{
	Safe_AddRef(m_pDevice);
}

HRESULT CPicking::Ready_Picking()
{
	return NOERROR;
}

HRESULT CPicking::Compute_MouseInLocalSpace(const CTransform* pTransform, _vec3 * pRay, _vec3 * pPivot)
{
	CPipeLine*	pPipeLine = CPipeLine::GetInstance();
	if (nullptr == pPipeLine)
		return E_FAIL;

	Safe_AddRef(pPipeLine);

	// 뷰스페이스 상의 마우스 좌표를 구한다.
	D3DDEVICE_CREATION_PARAMETERS			Parameters;
	m_pDevice->GetCreationParameters(&Parameters);	

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(Parameters.hFocusWindow, &ptMouse);

	// 투영스페이스 상의 마우스 좌표를 구하자.
	D3DVIEWPORT9		ViewPort;
	m_pDevice->GetViewport(&ViewPort);

	_vec4		vMousePoint;	

	vMousePoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePoint.z = 0.f;
	vMousePoint.w = 1.f;
	
	// 뷰스페이스 상의 레이와 피벗을 구하자.
	_matrix		matProj = pPipeLine->Get_Transform(D3DTS_PROJECTION);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	D3DXVec4Transform(&vMousePoint, &vMousePoint, &matProj);

	_vec3		vRay, vPivot;

	vPivot = _vec3(0.f, 0.f, 0.f);
	vRay = _vec3(vMousePoint.x, vMousePoint.y, vMousePoint.z) - vPivot;

	// 월드 스페이스 상의 레이와 피벗을 구하자.
	_matrix		matView = pPipeLine->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	D3DXVec3TransformNormal(&vRay, &vRay, &matView);
	D3DXVec3TransformCoord(&vPivot, &vPivot, &matView);

	// 로컬스페이스 이하동문
	_matrix	WorldInv = pTransform->Get_WorldMatrixInverse();

	D3DXVec3TransformNormal(&vRay, &vRay, &WorldInv);
	D3DXVec3TransformCoord(&vPivot, &vPivot, &WorldInv);

	*pRay = vRay;
	*pPivot = vPivot;

	Safe_Release(pPipeLine);

	return NOERROR;
}

_bool CPicking::Intersect_Polygon()
{
	return _bool();
}

CPicking * CPicking::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPicking* pInstance = new CPicking(pGraphic_Device);

	if (FAILED(pInstance->Ready_Picking()))
	{
		MSG_BOX("Failed while Creating CPicking");
		Safe_Release(pInstance);
	}

	

	return pInstance;
}



void CPicking::Free()
{
	Safe_Release(m_pDevice);
}
