// Map_Object.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Map_Object.h"
#include "afxdialogex.h"

#include "Management.h"
#include "Object_Tool.h"
#include "ToolView.h"
#include "MainFrm.h"

// CMap_Object 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMap_Object, CDialogEx)

CMap_Object::CMap_Object(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAP_OBJECT, pParent)
	, m_Angle(0)
	, m_dwScaleX(1.f)
	, m_dwScaleY(1.f)
	, m_dwScaleZ(1.f)
	, m_dwObjectXPosition(0)
	, m_dwObjectYPosition(0)
	, m_dwObjectZPosition(0)
{

}

CMap_Object::~CMap_Object()
{
	if (!m_umObject.empty())
	{
		//for (auto& iter : m_umObject)
		//{
		//	Safe_Delete(iter.second);
		//}
		m_umObject.clear();
	}

	if (!m_vecObject.empty())
	{
		//for (auto& iter : m_vecObject)
		//{
		//	Safe_Delete(iter);
		//}
		m_vecObject.clear();
	}

	DestroyWindow();
}

void CMap_Object::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, AngleBox);
	DDX_Control(pDX, IDC_SLIDER1, m_AngleControl);
	DDX_Text(pDX, IDC_EDIT2, m_Angle);
	DDX_Text(pDX, IDC_EDIT4, m_dwScaleX);
	DDX_Text(pDX, IDC_EDIT10, m_dwScaleY);
	DDX_Text(pDX, IDC_EDIT11, m_dwScaleZ);
	DDX_Control(pDX, IDC_LIST2, m_ListBox);
	DDX_Text(pDX, IDC_EDIT6, m_dwObjectXPosition);
	DDX_Text(pDX, IDC_EDIT12, m_dwObjectYPosition);
	DDX_Text(pDX, IDC_EDIT13, m_dwObjectZPosition);
}

BEGIN_MESSAGE_MAP(CMap_Object, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMap_Object::OnNMCustomdrawSlider1)
	ON_EN_CHANGE(IDC_EDIT2, &CMap_Object::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT10, &CMap_Object::OnEnChangeEdit10)
	ON_EN_CHANGE(IDC_EDIT4, &CMap_Object::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT11, &CMap_Object::OnEnChangeEdit11)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, &CMap_Object::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMap_Object::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &CMap_Object::OnBnClickedButton5)
END_MESSAGE_MAP()

BOOL CMap_Object::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_AngleControl.SetRange(0, 360);

	SetProtoType();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMap_Object::SetProtoType()
{
	CManagement* pManagement = CManagement::GetInstance();

	_matrix		LocalMatrix;

	_matrix		matScale, matRot;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationY(&matRot, D3DXToRadian(0.f));

	LocalMatrix = matScale * matRot;
	if (nullptr == pManagement)
		return;

	Safe_AddRef(pManagement);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (nullptr == pMainFrame)
		return;

	ProtoType* pProtoType = new ProtoType;

	if (FAILED(pManagement->Add_Prototype(0, L"Component_BrokenHouse", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/BrokenHouse/", L"BrokenHouse.x", &LocalMatrix))))
		return;

	if (FAILED(pManagement->Add_Prototype(L"GameObject_BrokenHouse", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"BrokenHouse";
	pProtoType->ProtoTypeName = L"Component_BrokenHouse";
	pProtoType->ObjectName = L"GameObject_BrokenHouse";

	m_ListBox.AddString(L"BrokenHouse");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_Burn_Car", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Car/BurnCar/", L"BurnCar.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Burn_Car", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"Burn_Car";
	pProtoType->ProtoTypeName = L"Component_Burn_Car";
	pProtoType->ObjectName = L"GameObject_Burn_Car";

	m_ListBox.AddString(L"Burn_Car");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_Burn_Car1", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Car/BurnCar1/", L"BurnCar1.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Burn_Car1", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"Burn_Car1";
	pProtoType->ProtoTypeName = L"Component_Burn_Car1";
	pProtoType->ObjectName = L"GameObject_Burn_Car1";

	m_ListBox.AddString(L"Burn_Car1");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_IceCreamVan", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Car/IceCreamVan/", L"IceCreamVan.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_IceCreamVan", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"IceCreamVan";
	pProtoType->ProtoTypeName = L"Component_IceCreamVan";
	pProtoType->ObjectName = L"GameObject_IceCreamVan";

	m_ListBox.AddString(L"IceCreamVan");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_Crane_Cabin", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Crane/", L"Crane_Cabin.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Crane_Cabin", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"Crane_Cabin";
	pProtoType->ProtoTypeName = L"Component_Crane_Cabin";
	pProtoType->ObjectName = L"GameObject_Crane_Cabin";

	m_ListBox.AddString(L"Crane_Cabin");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_Crane_Legs", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Crane/", L"Crane_Legs.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Crane_Legs", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"Crane_Legs";
	pProtoType->ProtoTypeName = L"Component_Crane_Legs";
	pProtoType->ObjectName = L"GameObject_Crane_Legs";

	m_ListBox.AddString(L"Crane_Legs");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));
	
	if (FAILED(pManagement->Add_Prototype(0, L"Component_BrokenFence", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Fence/", L"BrokenFence.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_BrokenFence", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"BrokenFence";
	pProtoType->ProtoTypeName = L"Component_BrokenFence";
	pProtoType->ObjectName = L"GameObject_BrokenFence";

	m_ListBox.AddString(L"BrokenFence");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_FenseSmall", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Fence/", L"FenceSmall.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_FenseSmall", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"FenseSmall";
	pProtoType->ProtoTypeName = L"Component_FenseSmall";
	pProtoType->ObjectName = L"GameObject_FenseSmall";

	m_ListBox.AddString(L"FenseSmall");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_House1", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/House/House1/", L"House1.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_House1", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"House1";
	pProtoType->ProtoTypeName = L"Component_House1";
	pProtoType->ObjectName = L"GameObject_House1";

	m_ListBox.AddString(L"House1");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_House2", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/House/House2/", L"House2.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_House2", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"House2";
	pProtoType->ProtoTypeName = L"Component_House2";
	pProtoType->ObjectName = L"GameObject_House2";

	m_ListBox.AddString(L"House2");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_House3", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/House/House3/", L"House3.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_House3", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"House3";
	pProtoType->ProtoTypeName = L"Component_House3";
	pProtoType->ObjectName = L"GameObject_House3";

	m_ListBox.AddString(L"House3");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_House4", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/House/House4/", L"House4.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_House4", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"House4";
	pProtoType->ProtoTypeName = L"Component_House4";
	pProtoType->ObjectName = L"GameObject_House4";

	m_ListBox.AddString(L"House4");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_House5", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/House/House5/", L"House5.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_House5", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"House5";
	pProtoType->ProtoTypeName = L"Component_House5";
	pProtoType->ObjectName = L"GameObject_House5";

	m_ListBox.AddString(L"House5");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_House6", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/House/House6/", L"House6.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_House6", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"House6";
	pProtoType->ProtoTypeName = L"Component_House6";
	pProtoType->ObjectName = L"GameObject_House6";

	m_ListBox.AddString(L"House6");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_Lighting_Street", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Lighting_Street/", L"Lighting_Street.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Lighting_Street", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"Lighting_Street";
	pProtoType->ProtoTypeName = L"Component_Lighting_Street";
	pProtoType->ObjectName = L"GameObject_Lighting_Street";

	m_ListBox.AddString(L"Lighting_Street");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_Lighting_Streetx2", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Lighting_Street/", L"Lighting_Streetx2.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Lighting_Streetx2", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"Lighting_Streetx2";
	pProtoType->ProtoTypeName = L"Component_Lighting_Streetx2";
	pProtoType->ObjectName = L"GameObject_Lighting_Streetx2";

	m_ListBox.AddString(L"Lighting_Streetx2");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_Manhole", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Manhole/", L"Manhole.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Manhole", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"Manhole";
	pProtoType->ProtoTypeName = L"Component_Manhole";
	pProtoType->ObjectName = L"GameObject_Manhole";

	m_ListBox.AddString(L"Manhole");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_Museum", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Museum/", L"Museum.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Museum", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"Museum";
	pProtoType->ProtoTypeName = L"Component_Museum";
	pProtoType->ObjectName = L"GameObject_Museum";

	m_ListBox.AddString(L"Museum");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_PrisonFense", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/PrisonTowerTop/", L"PrisonFenseBig.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_PrisonFense", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"PrisonFense";
	pProtoType->ProtoTypeName = L"Component_PrisonFense";
	pProtoType->ObjectName = L"GameObject_PrisonFense";

	m_ListBox.AddString(L"PrisonFense");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_PrisonFenseRound", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/PrisonTowerTop/", L"PrisonFenseRound.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_PrisonFenseRound", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"PrisonFenseRound";
	pProtoType->ProtoTypeName = L"Component_PrisonFenseRound";
	pProtoType->ObjectName = L"GameObject_PrisonFenseRound";

	m_ListBox.AddString(L"PrisonFenseRound");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_PrisonWallBig", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/PrisonTowerTop/", L"PrisonWallBig.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_PrisonWallBig", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"PrisonWallBig";
	pProtoType->ProtoTypeName = L"Component_PrisonWallBig";
	pProtoType->ObjectName = L"GameObject_PrisonWallBig";

	m_ListBox.AddString(L"PrisonWallBig");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_PrisonWallSmall", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/PrisonTowerTop/", L"PrisonWallSmall.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_PrisonWallSmall", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"PrisonWallSmall";
	pProtoType->ProtoTypeName = L"Component_PrisonWallSmall";
	pProtoType->ObjectName = L"GameObject_PrisonWallSmall";

	m_ListBox.AddString(L"PrisonWallSmall");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));


	if (FAILED(pManagement->Add_Prototype(0, L"Component_Sign", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Sign/", L"Sign.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Sign", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"Sign";
	pProtoType->ProtoTypeName = L"Component_Sign";
	pProtoType->ObjectName = L"GameObject_Sign";

	m_ListBox.AddString(L"Sign");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_Tree", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Tree/", L"Tree.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Tree", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"Tree";
	pProtoType->ProtoTypeName = L"Component_Tree";
	pProtoType->ObjectName = L"GameObject_Tree";

	m_ListBox.AddString(L"Tree");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_Wall", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Wall/", L"Wall.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_Wall", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"Wall";
	pProtoType->ProtoTypeName = L"Component_Wall";
	pProtoType->ObjectName = L"GameObject_Wall";

	m_ListBox.AddString(L"Wall");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_WallBig", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Wall/", L"WallBig.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_WallBig", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"WallBig";
	pProtoType->ProtoTypeName = L"Component_WallBig";
	pProtoType->ObjectName = L"GameObject_WallBig";

	m_ListBox.AddString(L"WallBig");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_MetalWall1", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Wall/MetalWall/MetalWall1/", L"MetalWall1.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_MetalWall1", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"MetalWall1";
	pProtoType->ProtoTypeName = L"Component_MetalWall1";
	pProtoType->ObjectName = L"GameObject_MetalWall1";

	m_ListBox.AddString(L"MetalWall1");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_MetalWall2", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Wall/MetalWall/MetalWall2/", L"MetalWall2.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_MetalWall2", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"MetalWall2";
	pProtoType->ProtoTypeName = L"Component_MetalWall2";
	pProtoType->ObjectName = L"GameObject_MetalWall2";

	m_ListBox.AddString(L"MetalWall2");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_MetalWall3", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Wall/MetalWall/MetalWall3/", L"MetalWall3.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_MetalWall3", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"MetalWall3";
	pProtoType->ProtoTypeName = L"Component_MetalWall3";
	pProtoType->ObjectName = L"GameObject_MetalWall3";

	m_ListBox.AddString(L"MetalWall3");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));

	if (FAILED(pManagement->Add_Prototype(0, L"Component_MetalWall4", CMesh_Static::Create(pMainFrame->GetToolView()->m_pDevice, L"../Client/Bin/Resource/Mesh/StaticMesh/Object/Wall/MetalWall/MetalWall4/", L"MetalWall4.x", &LocalMatrix))))
		return;
	if (FAILED(pManagement->Add_Prototype(L"GameObject_MetalWall4", CObject_Tool::Create(pMainFrame->GetToolView()->m_pDevice))))
		return;

	pProtoType->FileName = L"MetalWall4";
	pProtoType->ProtoTypeName = L"Component_MetalWall4";
	pProtoType->ObjectName = L"GameObject_MetalWall4";

	m_ListBox.AddString(L"MetalWall4");
	m_umObject.insert(ProtoTypeMAP::value_type(pProtoType->FileName, *pProtoType));
}

void CMap_Object::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	m_Angle = m_AngleControl.GetPos();
	UpdateData(FALSE);
	*pResult = 0;
}

void CMap_Object::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	UpdateData(true);

	m_AngleControl.SetPos(m_Angle);

	UpdateData(false);	
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMap_Object::OnEnChangeEdit10()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(true);
	
	m_vecScale = { m_dwScaleX, m_dwScaleY, m_dwScaleZ };

	UpdateData(false);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMap_Object::OnEnChangeEdit4()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(true);

	m_vecScale = { m_dwScaleX,m_dwScaleY,m_dwScaleZ };

	UpdateData(false);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMap_Object::OnEnChangeEdit11()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(true);

	m_vecScale = { m_dwScaleX,m_dwScaleY,m_dwScaleZ };

	UpdateData(false);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMap_Object::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return;

	Safe_AddRef(pManagement);

	UpdateData(true);

	_tchar* pProtoTypeName =new _tchar[MAX_PATH];
	m_ListBox.GetText(m_ListBox.GetCurSel(), pProtoTypeName);

	_matrix WorldMat, ScaleMat, RotationMat;

	m_vecScale = { m_dwScaleX,m_dwScaleY,m_dwScaleZ };
	m_vPosition = { (float)m_dwObjectXPosition, (float)m_dwObjectYPosition, (float)m_dwObjectZPosition };

	D3DXMatrixScaling(&ScaleMat, m_dwScaleX, m_dwScaleY, m_dwScaleZ);
	D3DXMatrixRotationY(&RotationMat, D3DXToRadian(m_Angle));

	WorldMat = ScaleMat * RotationMat;
	memcpy(&WorldMat.m[3][0], &m_vPosition, sizeof(_vec3));

	ProtoType tempType = find_if(m_umObject.begin(), m_umObject.end(), CTag_Finder(pProtoTypeName))->second;

	OBJECT tempObject;
	memcpy(&tempObject.TextInfo, &tempType,sizeof(ProtoType));
	tempObject.WorldMatrix = WorldMat;
	
	pManagement->Add_GameObjectToLayer(tempType.ObjectName, 0, tempType.FileName, &tempObject);

	m_vecObject.push_back(tempObject);

	UpdateData(false);

	delete pProtoTypeName;

	Safe_Release(pManagement);
}

void CMap_Object::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HANDLE			hFile = 0;
	_ulong			dwByte = 0;

	hFile = CreateFile(L"../Client/Bin/Data/ObjectMap.txt", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;
	
	for (auto& iter : m_vecObject)
	{
		_tchar FileName[256] = L"";
		lstrcpy(FileName ,iter.TextInfo.FileName);
		
		_tchar ProtoTypeName[256] = L"";
		lstrcpy(ProtoTypeName, iter.TextInfo.ProtoTypeName);

		_tchar ObjectName[256] = L"";
		lstrcpy(ObjectName, iter.TextInfo.ObjectName);


		int FileNameLength = lstrlen(FileName)+1;
		int ProtoTypeNameLength = lstrlen(ProtoTypeName)+1;
		int ObjectNameLength = lstrlen(ObjectName)+1;
		
		WriteFile(hFile, &FileName, sizeof(_tchar) * 256, &dwByte, nullptr);
		WriteFile(hFile, &ProtoTypeName, sizeof(_tchar) * 256, &dwByte, nullptr);
		WriteFile(hFile, &ObjectName, sizeof(_tchar) * 256, &dwByte, nullptr);
		WriteFile(hFile, &iter.WorldMatrix, sizeof(_matrix), &dwByte, nullptr);

	}

	CloseHandle(hFile);
}


void CMap_Object::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HANDLE			hFile = 0;
	_ulong			dwByte = 0;

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());

	hFile = CreateFile(L"../Client/MapData/ObjectMap.txt", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;

	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return;

	Safe_AddRef(pManagement);

	while(true)
	{
		OBJECT*	pObject = new OBJECT;
		
		_tchar FileName[256] = L"";
		_tchar ProtoTypeName[256] = L"";
		_tchar ObjectName[256] = L"";

		ReadFile(hFile, &FileName, sizeof(_tchar) * 256, &dwByte, nullptr);
		ReadFile(hFile, &ProtoTypeName, sizeof(_tchar) * 256, &dwByte, nullptr);
		ReadFile(hFile, &ObjectName, sizeof(_tchar) * 256, &dwByte, nullptr);
		ReadFile(hFile, &pObject->WorldMatrix, sizeof(_matrix), &dwByte, nullptr);

		pObject->TextInfo.FileName = FileName;
		pObject->TextInfo.ProtoTypeName = ProtoTypeName;
		pObject->TextInfo.ObjectName = ObjectName;

		if (nullptr == pObject)
			break;

		if (0 == dwByte)
			break;

		pManagement->Add_GameObjectToLayer(ObjectName, 0, FileName, pObject);

		m_vecObject.push_back(*(OBJECT*)&pObject);
	}

	Safe_Release(pManagement);

	CloseHandle(hFile);
}
