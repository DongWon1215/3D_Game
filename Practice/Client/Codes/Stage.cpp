#include "stdafx.h"
#include "..\Headers\Stage.h"
#include "Management.h"
#include "BossScene.h"
#include "Camera.h"
#include "Player.h"
#include "Monster.h"
#include "Boss.h"
#include "Progress.h"

USING(Client)

CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
	:CScene(pDevice)
{
}

HRESULT CStage::Ready_Scene()
{
	ShowCursor(false);
	//m_iRemaining = 0;
	//m_iWave = 3;

	if (FAILED(Ready_Light_Desc()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(L"Layer_Player")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(L"Layer_Effect")))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;

	return S_OK;
}

_int CStage::Update_Scene(_double TimeDelta)
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return -1;

	m_fWaitingTime -= 1.f * TimeDelta;

	if (m_fWaitingTime <= 0.f)
		m_fWaitingTime = 0.f;

	if (m_fWaitingTime == 0.f)
	{
		if (FAILED(Ready_Layer_Monster(L"Layer_Monster")))
			return E_FAIL;
	}

	Safe_AddRef(pManagement);
	if (pManagement->KeyDown(KEY_F1) || m_iWave > 3)
	{
		m_pLoading = CBossLoading::Create(GetDevice(), SCENE_BOSS);
		if (nullptr == m_pLoading)
			return -1;
	}
	vector<CGameObject*> TempVector = pManagement->Get_ObjectList(SCENE_STAGE, L"Layer_Monster");
	_uint ExistingMonster= TempVector.size();
	if (m_iRemaining == 0 && ExistingMonster == 0)
	{
		m_iWave++;
		SetMonsterNum();
		//m_fWaitingTime = 5.f;
		if (m_iWave <= 3)
		{
			if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Wave", SCENE_STAGE, L"Layer_UI", &m_iWave)))
				return E_FAIL;
		}
	}

	if (m_iWave < 3)
	{
		if (pManagement->KeyUp(KEY_UP))
		{
			//dynamic_cast<CMonster*>(pManagement->Get_GameObject(L"GameObject_Wave", SCENE_STAGE, L"Layer_Monster", ))->Set_Collider_Show();
			for (auto& iter : TempVector)
			{
				dynamic_cast<CMonster*>(iter)->Set_Collider_Show();
			}
		}
		if (pManagement->KeyUp(KEY_SPACE))
		{
			for (auto& iter : TempVector)
			{
				dynamic_cast<CMonster*>(iter)->Cheat();
			}
		}
	}


	dynamic_cast<CProgress*>(pManagement->Get_GameObject(L"GameObject_Progress", SCENE_STAGE, L"Layer_UI", 3))->Set_Progress(m_iRemaining);

	Safe_Release(pManagement);
	return _int();
}

_int CStage::LateUpdate_Scene(_double TimeDelta)
{
	CManagement* pManagement = CManagement::GetInstance();
	
	if (nullptr == pManagement)
		return -1;
	
	Safe_AddRef(pManagement);
	if (nullptr != m_pLoading)
	{
		if (100 == m_pLoading->Get_Complete())
		{
			CScene*	pScene = CBossScene::Create(GetDevice());
			if (nullptr == pScene)
				return -1;
	
			if (FAILED(pManagement->Ready_Current_Scene(pScene)))
				return -1;
		}
	}
	
	Safe_Release(pManagement);
	return _int();
}

HRESULT CStage::Render_Scene()
{
	return S_OK;
}

HRESULT CStage::Ready_Light_Desc()
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

HRESULT CStage::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);


	// For.Player
	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Player", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Pistol", SCENE_STAGE, pLayerTag)))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Riffle", SCENE_STAGE, pLayerTag)))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Sniper", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CStage::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);


	// For.Camera_Free
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));


	CameraDesc.vEye = _vec3(0.f,7.f,-10.f);
	CameraDesc.vAt = _vec3(0.f, -7.f, 10.f);
	CameraDesc.vAxisY = _vec3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = D3DXToRadian(90.0f);
	CameraDesc.fAspect = g_WINCX / _float(g_WINCY);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 450.0f;

	CameraDesc.fSpeedPerSec = 5.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(60.0f);

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Camera_Free", SCENE_STAGE, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CStage::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	vector<CGameObject*> TempVector = pManagement->Get_ObjectList(SCENE_STAGE, L"Layer_Monster");

	if (TempVector.size() <= 10 && m_iRemaining > 0)
	{
		for (size_t i = 0; i < 10 - TempVector.size(); ++i)
		{
			if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Minion", SCENE_STAGE, pLayerTag)))
				return E_FAIL;
	
			m_iRemaining--;
		}
	}
	if (TempVector.size() <= 0 && m_iWave == 3)
	{
		if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Boss", SCENE_STAGE, pLayerTag)))
			return E_FAIL;
		if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_BossHpBar", SCENE_STAGE, L"Layer_UI")))
			return E_FAIL;

		m_iWave++;
	}

	Safe_Release(pManagement);
	
	return S_OK;
}

HRESULT CStage::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_HPBar", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_BulletResiduum", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_AimingPoint", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Progress", SCENE_STAGE, pLayerTag)))
		return E_FAIL;
	
	
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CStage::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);


	// For.Terrain
	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Terrain", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Sky", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	HANDLE hFile;
	_ulong dwByte = 0;

	hFile = CreateFile(L"../Bin/Data/StageObjectMap.txt", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	while(true)
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

		if (FAILED(pManagement->Add_GameObjectToLayer(pObject->ObjectName, SCENE_STAGE, pLayerTag, pObject->WorldMatrix)))
			return E_FAIL;
	}

	Safe_Release(pManagement);
	return S_OK;
}

void CStage::SetMonsterNum()
{
	switch (m_iWave)
	{
	case 1:
		m_iRemaining = 30;
		break;
	case 2:
		m_iRemaining = 50;
		break;
	}
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage* pInstance = new CStage(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed while Creating CStage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStage::Free()
{
	Safe_Release(m_pLoading);

	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;

	Safe_AddRef(pManagement);

	pManagement->Clear_Scene(SCENE_STAGE);

	Safe_Release(pManagement);

	CScene::Free();
}