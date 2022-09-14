#include "stdafx.h"
#include "..\Headers\BossLoading.h"
#include "Management.h"

#include "Sky.h"
#include "Player.h"
#include "Terrain.h"
#include "Camera_Free.h"
#include "Pistol.h"
#include "Riffle.h"
#include "Sniper.h"
#include "Boss.h"
#include "HPBar.h"
#include "Bullet.h"
#include "HitScreen.h"
#include "Progress.h"
#include "BulletResiduum.h"
#include "Bleeding.h"

USING(Client)

CBossLoading::CBossLoading(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
}

HRESULT CBossLoading::Ready_Loading(SCENEID eSceneID)
{
	m_eSceneID = eSceneID;

	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 1, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_uint CBossLoading::Thread_Main(void * pArg)
{
	CBossLoading*		pLoading = (CBossLoading*)pArg;

	EnterCriticalSection(&pLoading->m_CS);

	HRESULT			hr;

	switch (pLoading->Get_SceneID())
	{
	case SCENE_BOSS:
		hr = pLoading->Ready_Stage();
		break;
	}

	if (FAILED(hr))
		return -1;

	LeaveCriticalSection(&pLoading->m_CS);

	return _uint(0);
}

HRESULT CBossLoading::Ready_Stage()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	//스테이지에 필요한 오브젝트 프로토 타입 세팅
	//카메라 세팅
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Camera_Free", CCamera_Free::Create(m_pDevice))))
		return E_FAIL;
	
	//플레이어 세팅
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Player", CPlayer::Create(m_pDevice))))
		return E_FAIL;
	
	//지형 세팅
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Terrain", CTerrain::Create(m_pDevice))))
		return E_FAIL;
	
	//무기 세팅
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Pistol", CPistol::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Riffle", CRiffle::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Sniper", CSniper::Create(m_pDevice))))
		return E_FAIL;
	
	//스카이 박스 세팅
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Sky", CSky::Create(m_pDevice))))
		return E_FAIL;
	
	//몬스터 세팅
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Boss", CBoss::Create(m_pDevice))))
		return E_FAIL;
	
	//총알 세팅
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Bullet", CBullet::Create(m_pDevice))))
		return E_FAIL;
	
	//이펙트 세팅
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Bleeding", CBleeding::Create(m_pDevice))))
		return E_FAIL;
	
	// UI 세팅
	if (FAILED(pManagement->Add_Prototype(L"GameObject_HitScreen", CHitScreen::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Progress", CProgress::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_BulletResiduum", CBulletResiduum::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_HPBar", CHPBar::Create(m_pDevice))))
		return E_FAIL;


	// 오브젝트 매쉬 프로토 타입 생성
	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Shader_Terrain", CShader::Create(m_pDevice, L"../Bin/Shader/Shader_Terrain.fx"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Shader_Mesh", CShader::Create(m_pDevice, L"../Bin/Shader/Shader_Mesh.fx"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Shader_Sky", CShader::Create(m_pDevice, L"../Bin/Shader/Shader_Sky.fx"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Shader_Default", CShader::Create(m_pDevice, L"../Bin/Shader/Shader_Default.fx"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Frustum", CFrustum::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Boss_Navigation", CNavigation::Create(m_pDevice, L"../Bin/Data/BossNavigation.dat"))))
		return E_FAIL;

	_matrix		LocalMatrix;

	_matrix		matScale, matRot;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationY(&matRot, D3DXToRadian(0.0f));

	LocalMatrix = matScale * matRot;

	// 플레이어 매쉬
	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Mesh_Player", CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resource/Mesh/DynamicMesh/Player/", L"Player.x", &LocalMatrix))))
		return E_FAIL;

	// 무기 매쉬
	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Mesh_Pistol", CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resource/Mesh/DynamicMesh/Weapon/Pistol/", L"Pistol.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Mesh_Riffle", CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resource/Mesh/DynamicMesh/Weapon/Riffle/", L"Riffle.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Mesh_Sniper", CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resource/Mesh/DynamicMesh/Weapon/Sniper/", L"Sniper.x", &LocalMatrix))))
		return E_FAIL;

	//버텍스 프로토 타입 세팅
	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_VIBuffer_Terrain", CVIBuffer_Terrain::Create(m_pDevice, L"../Bin/Resource/Texture/Terrain/Terrain.bmp", 1.f))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_VIBuffer_Cube", CVIBuffer_Cube::Create(m_pDevice))))
		return E_FAIL;

	//텍스쳐 프로토 타입 세팅
	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Texture_Terrain", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/Terrain/Multi%d.bmp", 4))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Texture_Filter", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/Terrain/Filter.bmp"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Texture_HPBar", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/UI/HP_Bar.png"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Texture_Score", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/UI/Money.png"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Texture_UI", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/UI/UI%d.png", 2))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Texture_Bleeding", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/Effect/Blood/Blood%d.tga", 16))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Texture_HitScreen", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/Effect/Player_Effect.png"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Texture_Sky", CTexture::Create(m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Resource/Texture/SkyBox/SkyBox%d.dds", 2))))
		return E_FAIL;


	//콜라이더 프로토 타입 세팅
	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Collider", CCollider::Create(m_pDevice, CCollider::TYPE_AABB))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Collider_OBB", CCollider::Create(m_pDevice, CCollider::TYPE_OBB))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Collider_SPHERE", CCollider::Create(m_pDevice, CCollider::TYPE_SPHERE))))
		return E_FAIL;



	// 몬스터의 경우 회전을 시켜줘야 함
	D3DXMatrixRotationY(&matRot, D3DXToRadian(180.0f));
	LocalMatrix = matScale * matRot;

	if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Mesh_Boss", CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resource/Mesh/DynamicMesh/Boss/", L"Boss.x", &LocalMatrix))))
		return E_FAIL;

	//이펙트 텍스쳐 프로토 타입 저장

	m_iComplete = 100;

	Safe_Release(pManagement);

	return S_OK;
}

CBossLoading * CBossLoading::Create(LPDIRECT3DDEVICE9 pDevice, SCENEID eSceneID)
{
	CBossLoading* pInstance = new CBossLoading(pDevice);

	if (FAILED(pInstance->Ready_Loading(eSceneID)))
	{
		MSG_BOX("CBossLoading Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBossLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITY);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CS);
}
