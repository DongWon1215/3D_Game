
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "Management.h"

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "Camera_Tool.h"
#include "Terrain.h"
#include "VIBuffer_Terrain.h"
#include "input.h"
#include "Object_Tool.h"
#include "Scene_Tool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	: m_pDevice(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CToolView::~CToolView()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);

}

void CToolView::SetTerrainTag(const _tchar* pTerrainTag)
{
	lstrcpy(m_strTerrain,pTerrainTag);
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());

	if (nullptr == pMainFrame)
		return;
	
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);

	m_pDevice->BeginScene();


	m_pRenderer->Render_RenderList();
	m_pManagement->Render_Current_Scene();

	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, m_hWnd, nullptr);

	//Invalidate(false);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

CGameObject * CToolView::GetTerrainObject()
{
	return m_pManagement->Get_GameObject(L"GameObject_Terrain", 0, m_strTerrain, 0);
}

CGameObject * CToolView::GetGameObject(const _tchar* ObjectTag, _uint iIndex)
{
	return m_pManagement->Get_GameObject(L"GameObject_Object", 0, ObjectTag, iIndex);
}

void CToolView::UpdateInputDevice()
{
	m_pManagement->Set_Input_State();
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

HRESULT CToolView::Ready_ProtoType()
{
	if (FAILED(m_pManagement->Add_Prototype(0, L"Component_Shader_Default", CShader::Create(m_pDevice, L"Shader/Shader_Default.fx"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Prototype(L"GameObject_Camera_Free", CCamera_Tool::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(m_pManagement->Add_Prototype(L"GameObject_Terrain", CTerrain::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(m_pManagement->Add_Prototype(L"GameObject_Object", CObject_Tool::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Prototype(0, L"Component_Shader_Terrain", CShader::Create(m_pDevice, L"Shader/Shader_Terrain.fx"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Prototype(0, L"Component_Shader_Mesh", CShader::Create(m_pDevice, L"Shader/Shader_Mesh.fx"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Prototype(0, L"Component_Texture_Filter", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"Resource/Texture/Filter.bmp"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Prototype(0, L"Component_Texture_Brush", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"Resource/Texture/Brush.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Prototype(0, L"Component_Transform", CTransform::Create(m_pDevice))))
		return E_FAIL;
	
	if (FAILED(m_pManagement->Add_Prototype(0, L"Component_Texture_Terrain", CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"Resource/Texture/Grass_%d.tga", 2))))
		return E_FAIL;

	m_pManagement->Reserve_Light_Manager(1);

	D3DLIGHT9		LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
	LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Direction = _vec3(1.f, -1.f, 1.f);

	m_pManagement->Add_Light(m_pDevice, &LightDesc);

	return S_OK;
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	if(nullptr == pMainFrm)
		return;
	RECT rcMain = {};
	pMainFrm->GetWindowRect(&rcMain);
	::SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	RECT rcView = {};
	GetClientRect(&rcView);

	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	pMainFrm->SetWindowPos(
		nullptr, 0, 0, 800 + iGapX, 600 + iGapY, SWP_NOZORDER);

	

	m_pManagement = CManagement::GetInstance();
	Safe_AddRef(m_pManagement);
	m_pManagement->Ready_Engine(1);
	m_pManagement->Ready_Device(m_hWnd, CDevice::MODE_WIN, 800, 600, &m_pDevice);
	m_pManagement->Ready_Input(::AfxGetApp()->m_hInstance, ::AfxGetApp()->GetMainWnd()->m_hWnd);
	if (FAILED(m_pManagement->Ready_Current_Scene(CScene_Tool::Create(m_pDevice))))
		return;
	if (FAILED(m_pManagement->Add_Prototype(0, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pDevice))))
		return;
	Ready_ProtoType();

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));


	CameraDesc.vEye = _vec3(0.0f, 7.0f, -10.0f);
	CameraDesc.vAt = _vec3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _vec3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = 800 / _float(600);
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 500.0f;

	CameraDesc.fSpeedPerSec = 5.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);

	m_pManagement->Add_GameObjectToLayer(L"GameObject_Camera_Free", 0, L"Camera_Tool", &CameraDesc);

	//m_pManagement->Add_GameObjectToLayer(L"GameObject_Terrain", 0, L"Terrain_Default");
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.


}



void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pManagement->Set_Input_State();
	CView::OnMouseMove(nFlags, point);
}
