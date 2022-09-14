#include "stdafx.h"
#include "..\Headers\Loading.h"
#include "Management.h"
#include "Monster.h"

// 오브젝트 헤더 추가
#include "Sky.h"
#include "Player.h"
#include "Terrain.h"
#include "Camera_Free.h"
#include "Pistol.h"
#include "Riffle.h"
#include "Sniper.h"
#include "House_Broken.h"
#include "Burning_Car.h"
#include "Burning_Car_2.h"
#include "Crane_Cabin.h"
#include "Crane_Leg.h"
#include "Fence.h"
#include "Fence_Broken.h"
#include "House.h"
#include "House2.h"
#include "House3.h"
#include "House4.h"
#include "House5.h"
#include "House6.h"
#include "IceCream_Van.h"
#include "Lighting_Street.h"
#include "Lighting_Street2.h"
#include "ManHole.h"
#include "MetalWall.h"
#include "MetalWall2.h"
#include "MetalWall3.h"
#include "MetalWall4.h"
#include "PrisonFence.h"
#include "PrisonFense_Round.h"
#include "PrisonWall_Big.h"
#include "PrisonWall_Small.h"
#include "Sign.h"
#include "Tree.h"
#include "Wall.h"
#include "Wall_Big.h"
#include "Museum.h"
#include "Bullet.h"
#include "HitScreen.h"
#include "Progress.h"
#include "BulletResiduum.h"
#include "Bleeding.h"
#include "Boss.h"
#include "HPBar.h"
#include "BossHpBar.h"
#include "AmingPoint.h"
#include "Wave.h"


USING(Client)

CLoading::CLoading(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
}

HRESULT CLoading::Ready_Loading(SCENEID eSceneID)
{
	m_eSceneID = eSceneID;

	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 1, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_uint CLoading::Thread_Main(void * pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	EnterCriticalSection(&pLoading->m_CS);

	HRESULT			hr;

	switch (pLoading->Get_SceneID())
	{
	case SCENE_STAGE:
		hr = pLoading->Ready_Stage();
		break;
	case SCENE_BOSS:
		break;
	}

	if (FAILED(hr))
		return -1;

	LeaveCriticalSection(&pLoading->m_CS);

	return _uint(0);
}

HRESULT CLoading::Ready_Stage()
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
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Minion", CMonster::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_Boss", CBoss::Create(m_pDevice))))
		return E_FAIL;

	//총알 세팅
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Bullet", CBullet::Create(m_pDevice))))
		return E_FAIL;

	// 오브젝트 프로토 타입 객체 생성
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Burn_Car", CBurning_Car::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Burn_Car1", CBurning_Car2::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_IceCreamVan", CIceCream_Van::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Crane_Cabin", CCrane_Cabin::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Crane_Legs", CCrane_Leg::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_BrokenFence", CFence_Broken::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_BrokenHouse", CHouse_Broken::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_FenseSmall", CFence::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_House1", CHouse::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_House2", CHouse2::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_House3", CHouse3::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_House4", CHouse4::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_House5", CHouse5::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_House6", CHouse6::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Lighting_Street", CLighting_Street::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_Lighting_Streetx2", CLighting_Street2::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Manhole", CManHole::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_Museum", CMuseum::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_MetalWall1", CMetalWall::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_MetalWall2", CMetalWall2::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_MetalWall3", CMetalWall3::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_MetalWall4", CMetalWall4::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_PrisonFense", CPrisonFence::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_PrisonFenseRound", CPrisonFense_Round::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_PrisonWallBig", CPrisonWall_Big::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_PrisonWallSmall", CPrisonWall_Small::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_Sign", CSign::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_Tree", CTree::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_Wall", CWall::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_WallBig", CWall_Big::Create(m_pDevice))))
		return E_FAIL;


	// 오브젝트 프로토 타입 객체 생성 완료

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

	if (FAILED(pManagement->Add_Prototype(L"GameObject_BossHpBar", CBossHpBar::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(L"GameObject_AimingPoint", CAimingPoint::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_Wave", CWave::Create(m_pDevice))))
		return E_FAIL;
	

	// 오브젝트 매쉬 프로토 타입 생성
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Shader_Terrain", CShader::Create(m_pDevice, L"../Bin/Shader/Shader_Terrain.fx"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Shader_Mesh", CShader::Create(m_pDevice, L"../Bin/Shader/Shader_Mesh.fx"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Shader_Sky", CShader::Create(m_pDevice, L"../Bin/Shader/Shader_Sky.fx"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Shader_Default", CShader::Create(m_pDevice, L"../Bin/Shader/Shader_Default.fx"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Frustum", CFrustum::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Stage_Navigation", CNavigation::Create(m_pDevice, L"../Bin/Data/Navigation.dat"))))
		return E_FAIL;

	//if (FAILED(pManagement->Add_Prototype(SCENE_BOSS, L"Component_Boss_Navigation", CNavigation::Create(m_pDevice, L"../Bin/Data/BossNavigation.dat"))))
	//	return E_FAIL;

	_matrix		LocalMatrix;

	_matrix		matScale, matRot;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationY(&matRot, D3DXToRadian(0.0f));

	LocalMatrix = matScale * matRot;

	// 플레이어 매쉬
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Player", CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resource/Mesh/DynamicMesh/Player/", L"Player.x", &LocalMatrix))))
		return E_FAIL;


	// 무기 매쉬
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Pistol", CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resource/Mesh/DynamicMesh/Weapon/Pistol/", L"Pistol.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Riffle", CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resource/Mesh/DynamicMesh/Weapon/Riffle/", L"Riffle.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Sniper", CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resource/Mesh/DynamicMesh/Weapon/Sniper/", L"Sniper.x", &LocalMatrix))))
		return E_FAIL;
	
	// 게임 오브젝트 매쉬
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Fence_Broken", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Fence/", L"BrokenFence.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_House_Broken", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/BrokenHouse/", L"BrokenHouse.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Burning_Car", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Car/BurnCar/", L"BurnCar.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Burning_Car2", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Car/BurnCar1/", L"BurnCar1.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Crane_Cabin", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Crane/", L"Crane_Cabin.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Crane_Leg", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Crane/", L"Crane_Legs.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Fence", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Fence/", L"FenceSmall.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_House", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/House/House1/", L"House1.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_House2", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/House/House2/", L"House2.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_House3", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/House/House3/", L"House3.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_House4", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/House/House4/", L"House4.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_House5", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/House/House5/", L"House5.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_House6", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/House/House6/", L"House6.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_IceCream_Van", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Car/IceCreamVan/", L"IceCreamVan.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Lighting_Street", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Lighting_Street/", L"Lighting_Street.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Lighting_Street2", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Lighting_Street/", L"Lighting_Streetx2.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_ManHole", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/ManHole/", L"ManHole.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Museum", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Museum/", L"Museum.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_MetalWall", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Wall/MetalWall/MetalWall1/", L"MetalWall1.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_MetalWall2", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Wall/MetalWall/MetalWall2/", L"MetalWall2.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_MetalWall3", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Wall/MetalWall/MetalWall3/", L"MetalWall3.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_MetalWall4", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Wall/MetalWall/MetalWall4/", L"MetalWall4.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_PrisonFence", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/PrisonTowerTop/", L"PrisonFenseBig.x", &LocalMatrix))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_PrisonFense_Round", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/PrisonTowerTop/", L"PrisonFenseRound.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_PrisonWall_Big", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/PrisonTowerTop/", L"PrisonWallBig.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_PrisonWall_Small", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/PrisonTowerTop/", L"PrisonWallSmall.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Sign", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Sign/", L"Sign.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Tree", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Tree/", L"Tree.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Wall", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Wall/", L"Wall.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Wall_Big", CMesh_Static::Create(m_pDevice, L"../Bin/Resource/Mesh/StaticMesh/Object/Wall/", L"WallBig.x", &LocalMatrix))))
		return E_FAIL;

	// 그 외의 프로토 타입

	//버텍스 프로토 타입 세팅
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_VIBuffer_Terrain", CVIBuffer_Terrain::Create(m_pDevice, L"../Bin/Resource/Texture/Terrain/Terrain.bmp", 1.f))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_VIBuffer_Cube", CVIBuffer_Cube::Create(m_pDevice))))
		return E_FAIL;

	//텍스쳐 프로토 타입 세팅
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Texture_Terrain", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/Terrain/Multi%d.bmp", 4))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Texture_Filter", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/Terrain/Filter.bmp"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Texture_HPBar", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/UI/HP_Bar.png"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Texture_BossHPBar", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/UI/HP_Bar.png"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Texture_Score", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/UI/Money.png"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Texture_UI", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/UI/UI%d.png",2))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Texture_Bleeding", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/Effect/Blood/Blood%d.tga",16))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Texture_HitScreen", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/Effect/Player_Effect.png"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Texture_AimingPoint", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/UI/AimingPoint.png"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Texture_Circle", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resource/Texture/UI/Circle.png"))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Texture_Sky", CTexture::Create(m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Resource/Texture/SkyBox/SkyBox%d.dds", 2))))
		return E_FAIL;


	//콜라이더 프로토 타입 세팅
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Collider", CCollider::Create(m_pDevice,CCollider::TYPE_AABB))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Collider_OBB", CCollider::Create(m_pDevice, CCollider::TYPE_OBB))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Collider_SPHERE", CCollider::Create(m_pDevice, CCollider::TYPE_SPHERE))))
		return E_FAIL;



	// 몬스터의 경우 회전을 시켜줘야 함
	D3DXMatrixRotationY(&matRot, D3DXToRadian(180.0f));
	LocalMatrix = matScale * matRot;

	// 몬스터 매쉬
	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Minion", CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resource/Mesh/DynamicMesh/Monster/", L"Monster.x", &LocalMatrix))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype(SCENE_STAGE, L"Component_Mesh_Boss", CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resource/Mesh/DynamicMesh/Boss/", L"Boss.x", &LocalMatrix))))
		return E_FAIL;

	//이펙트 텍스쳐 프로토 타입 저장

	m_iComplete = 100;

	Safe_Release(pManagement);

	return S_OK;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pDevice, SCENEID eSceneID)
{
	CLoading* pInstance = new CLoading(pDevice);

	if (FAILED(pInstance->Ready_Loading(eSceneID)))
	{
		MSG_BOX("CLoading Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITY);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CS);
}