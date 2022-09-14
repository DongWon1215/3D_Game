#include "stdafx.h"
#include "..\Headers\BossScene.h"
#include "Management.h"

#include "Camera.h"
#include "Player.h"
#include "Boss.h"

USING(Client)

CBossScene::CBossScene(LPDIRECT3DDEVICE9 pDevice)
	:CScene(pDevice)
{
}

HRESULT CBossScene::Ready_Scene()
{
	if (FAILED(Ready_Light_Desc()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(L"Layer_Player")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(L"Layer_Monster")))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(L"Layer_Effect")))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;

	return S_OK;
}

_int CBossScene::Update_Scene(_double TimeDelta)
{
	return _int();
}

_int CBossScene::LateUpdate_Scene(_double TimeDelta)
{
	return _int();
}

HRESULT CBossScene::Render_Scene()
{
	return S_OK;
}

HRESULT CBossScene::Ready_Light_Desc()
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Reserve_Light_Manager(1)))
		return E_FAIL;

	D3DLIGHT9		LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
	LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(pManagement->Add_Light(GetDevice(), &LightDesc)))
		return E_FAIL;
	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CBossScene::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);


	// For.Player
	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Player", SCENE_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Pistol", SCENE_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Riffle", SCENE_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Sniper", SCENE_BOSS, pLayerTag)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CBossScene::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);


	// For.Camera_Free
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));


	CameraDesc.vEye = _vec3(0.f, 7.f, -10.f);
	CameraDesc.vAt = _vec3(0.f, -7.f, 10.f);
	CameraDesc.vAxisY = _vec3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = D3DXToRadian(90.0f);
	CameraDesc.fAspect = g_WINCX / _float(g_WINCY);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 450.0f;

	CameraDesc.fSpeedPerSec = 5.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(60.0f);

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Camera_Free", SCENE_BOSS, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CBossScene::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Boss", SCENE_BOSS, pLayerTag)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CBossScene::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CBossScene::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_HPBar", SCENE_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_BulletResiduum", SCENE_BOSS, pLayerTag)))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CBossScene::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);


	// For.Terrain
	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Terrain", SCENE_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Sky", SCENE_BOSS, pLayerTag)))
		return E_FAIL;

	HANDLE hFile;
	_ulong dwByte = 0;

	hFile = CreateFile(L"../Bin/Data/BossObjectMap.txt", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	while (true)
	{
		OBJECT* pObject = new OBJECT;
		_tchar FileName[MAX_PATH] = L"";
		_tchar ProtoTypeName[MAX_PATH] = L"";
		_tchar ObjectName[MAX_PATH] = L"";


		ReadFile(hFile, &FileName, sizeof(_tchar) * 256, &dwByte, nullptr);
		ReadFile(hFile, &ProtoTypeName, sizeof(_tchar) * 256, &dwByte, nullptr);
		ReadFile(hFile, &ObjectName, sizeof(_tchar) * 256, &dwByte, nullptr);
		ReadFile(hFile, &pObject->WorldMatrix, sizeof(_matrix), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		pObject->FileName = FileName;
		pObject->ProtoTypeName = ProtoTypeName;
		pObject->ObjectName = ObjectName;

		if (FAILED(pManagement->Add_GameObjectToLayer(pObject->ObjectName, SCENE_BOSS, pLayerTag, pObject->WorldMatrix)))
			return E_FAIL;
	}

	Safe_Release(pManagement);
	return S_OK;
}

CBossScene * CBossScene::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBossScene* pInstance = new CBossScene(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed while Creating CBossScene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossScene::Free()
{
	CScene::Free();
}
