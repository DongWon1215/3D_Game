#include "stdafx.h"
#include "Camera_Tool.h"
#include "Management.h"
#include "ToolView.h"

#include "MainFrm.h"

CCamera_Tool::CCamera_Tool(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Tool::CCamera_Tool(const CCamera_Tool & rhs)
	: CCamera(rhs)
	,m_pToolView(rhs.m_pToolView)
{
}

HRESULT CCamera_Tool::Ready_GameObject_Prototype()
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());

	m_pToolView = pMainFrame->GetToolView();
	return S_OK;
}

HRESULT CCamera_Tool::Ready_GameObject(void * pArg)
{
	if (FAILED(CCamera::Ready_GameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CCamera_Tool::Update_GameObject(_double TimeDelta)
{
	POINT		ptMouse = { 800 >> 1, 600 >> 1 };
	
	ClientToScreen(m_pToolView->GetSafeHwnd(), &ptMouse);

	//SetCursorPos(ptMouse.x, ptMouse.y);


	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_pTransformCom->Go_Straight(TimeDelta * 0.5f);
	}
	
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_Backward(TimeDelta* 0.5f);
	}
	
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pTransformCom->Go_Left(TimeDelta* 0.5f);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pTransformCom->Go_Right(TimeDelta* 0.5f);
	}

	_long		MouseMove = 0;
	if (GetAsyncKeyState(MK_RBUTTON) & 0x8000)
	{
		if (MouseMove = pManagement->Get_DIMMove(CInput::DIM_X))
		{
			m_pTransformCom->Rotation_Axis(&_vec3(0.f, 1.f, 0.f), TimeDelta * (MouseMove * 0.2f));
		}

		if (MouseMove = pManagement->Get_DIMMove(CInput::DIM_Y))
		{
			m_pTransformCom->Rotation_Axis(&m_pTransformCom->Get_State(CTransform::STATE_RIGHT), TimeDelta * (MouseMove * 0.2f));
		}
	}

	Safe_Release(pManagement);

	return CCamera::Update_GameObject(TimeDelta);
}

_int CCamera_Tool::LateUpdate_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CCamera_Tool::Render_GameObject()
{
	return S_OK;
}

CCamera_Tool * CCamera_Tool::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Tool* pInstance = new CCamera_Tool(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Tool::Clone_GameObject(void * pArg)
{
	CCamera_Tool* pInstance = new CCamera_Tool(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Tool::Free()
{
	Safe_Release(m_pTransformCom);

	CCamera::Free();
}