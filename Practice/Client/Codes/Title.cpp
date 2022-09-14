#include "stdafx.h"
#include "../Headers/Title.h"
#include "BackGround.h"
#include "Stage.h"

#include "Management.h"

USING(Client)

CTitle::CTitle(LPDIRECT3DDEVICE9 pDevice)
	:CScene(pDevice)
{
}

HRESULT CTitle::Ready_Scene()
{
	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	m_pLoading = CLoading::Create(GetDevice(), SCENE_STAGE);
	if (nullptr == m_pLoading)
		return E_FAIL;
	return S_OK;
}

_int CTitle::Update_Scene(_double TimeDelta)
{
	return _int();
}

_int CTitle::LateUpdate_Scene(_double TimeDelta)
{
	if (100 == m_pLoading->Get_Complete())
	{
		CManagement*	pManagement = CManagement::GetInstance();
		if (nullptr == pManagement)
			return -1;

		Safe_AddRef(pManagement);

		CScene*	pScene = CStage::Create(GetDevice());
		if (nullptr == pScene)
			return -1;

		if (FAILED(pManagement->Ready_Current_Scene(pScene)))
			return -1;

		Safe_Release(pScene);

		Safe_Release(pManagement);

		return 0;
	}

	return _int();
}

HRESULT CTitle::Render_Scene()
{
	return S_OK;
}

HRESULT CTitle::Ready_GameObject_Prototype()
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	// For.Prototype_GameObject_BackGround
	if (FAILED(pManagement->Add_Prototype(L"Prototype_GameObject_BackGround", CBackGround::Create(GetDevice()))))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CTitle::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(L"Prototype_GameObject_BackGround", SCENE_LOGO, pLayerTag)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

CTitle * CTitle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTitle* pInstance = new CTitle(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed while Creating CTitle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTitle::Free()
{
	Safe_Release(m_pLoading);

	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;

	Safe_AddRef(pManagement);

	pManagement->Clear_Scene(SCENE_LOGO);

	Safe_Release(pManagement);

	CScene::Free();
}
