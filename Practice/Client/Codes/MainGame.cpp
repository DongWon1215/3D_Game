#include "stdafx.h"
#include "..\Headers\MainGame.h"
#include "Title.h"

USING(Client)

CMainGame::CMainGame()
{

}

// 내게임에 필요한 초기화과정을 거친다.
HRESULT CMainGame::Ready_MainApp()
{
	if (FAILED(Ready_Default_Setting(CDevice::MODE_WIN, g_WINCX, g_WINCY)))
		return E_FAIL;
	
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;
	
	if (FAILED(Ready_Current_Scene(SCENE_LOGO)))
		return E_FAIL;
	
	return NOERROR;
}

int CMainGame::Update_MainApp(double TimeDelta)
{

	if (nullptr == m_pManagement)
		return -1;
	
	if (FAILED(m_pManagement->Set_Input_State()))
		return -1;

	if (m_pManagement->KeyDown(KEY_LEFT))
		m_pRenderer->Set_RenderTarget_Show();

	return 	m_pManagement->Update_Current_Scene(TimeDelta);
}
HRESULT CMainGame::Render_MainApp()
{
	if (nullptr == m_pDevice ||
		nullptr == m_pManagement ||
		nullptr == m_pRenderer)
		return E_FAIL;
	
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	m_pDevice->BeginScene();
	
	m_pRenderer->Render_RenderList();
	m_pManagement->Render_Current_Scene();
	
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, 0, nullptr);

	return NOERROR;
}

HRESULT CMainGame::Ready_Default_Setting(CDevice::MODE eMode, _uint iWinCX, _uint iWinCY)
{
	 //For.Management
	if (nullptr == m_pManagement)
	{
		m_pManagement = CManagement::GetInstance();
		if (nullptr == m_pManagement)
			return E_FAIL;
	
		Safe_AddRef(m_pManagement);
	}
	
	// 엔진에서 사용할 수 있는 객체들의 생성작업을 한다.
	if (FAILED(m_pManagement->Ready_Engine(SCENE_END)))
		return E_FAIL;
	
	// For.Graphic_Device
	if (FAILED(m_pManagement->Ready_Device(g_hWnd, eMode, iWinCX, iWinCY, &m_pDevice)))
		return E_FAIL;
	
	// For.Input
	if (FAILED(m_pManagement->Ready_Input(g_hInst, g_hWnd)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainGame::Ready_Current_Scene(SCENEID eSceneID)
{
	if (nullptr == m_pManagement)
		return E_FAIL;
	
	CScene*		pScene = nullptr;
	
	switch (eSceneID)
	{
	case SCENE_LOGO:
		pScene = CTitle::Create(m_pDevice);
		break;
	case SCENE_STAGE:
		break;
	}
	
	if (nullptr == pScene)
		return E_FAIL;
	
	if (FAILED(m_pManagement->Ready_Current_Scene(pScene)))
		return E_FAIL;
	
	Safe_Release(pScene);

	return NOERROR;
}

HRESULT CMainGame::Ready_Component_Prototype()
{
	if (nullptr == m_pManagement)
		return E_FAIL;
	
	// For.Component_Prototype For.Static Scene
	
	// For.Component_Renderer
	if (FAILED(m_pManagement->Add_Prototype(SCENE_STATIC, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pDevice))))
		return E_FAIL;
	
	// For.Component_VIBuffer_Rect
	if (FAILED(m_pManagement->Add_Prototype(SCENE_STATIC, L"Component_VIBuffer_Rect", CVIBuffer_Rect::Create(m_pDevice))))
		return E_FAIL;
	
	// For.Component_Texture_Default
	if (FAILED(m_pManagement->Add_Prototype(SCENE_STATIC, L"Component_Texture_Default", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/LogoBack%d.tga",3))))
		return E_FAIL;
	
	// For.Component_Shader_Default
	if (FAILED(m_pManagement->Add_Prototype(SCENE_STATIC, L"Component_Shader_Default", CShader::Create(m_pDevice, L"../Bin/Shader/Shader_Default.fx"))))
		return E_FAIL;
	
	Safe_AddRef(m_pRenderer);

	return NOERROR;
}

CMainGame * CMainGame::Create()
{
	CMainGame* pInstance = new CMainGame();
	
	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("Failed while Creating CMainGame");
		Safe_Release(pInstance);
	}
	
	return pInstance;
	return nullptr;
}

void CMainGame::Free()
{

	Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);
	
	CManagement::Release_Engine();


}