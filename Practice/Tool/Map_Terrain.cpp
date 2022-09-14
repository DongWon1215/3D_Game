// Map_Terrain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Map_Terrain.h"
#include "afxdialogex.h"
#include "Management.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"
#include "Camera_Tool.h"

// CMap_Terrain 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMap_Terrain, CDialogEx)

CMap_Terrain::CMap_Terrain(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAP_TERRAIN, pParent)
	, XVertex(0)
	, ZVertex(0)
	, m_MouseX(0)
	, m_MouseY(0)
	, m_MouseZ(0)
	, m_dwYMax(0)
	, m_fInterval(0)
	, m_iType(0)
	, m_szTerrainName(_T(""))
	, m_fRange(0)
	, m_iDirection(0)
{

}

CMap_Terrain::~CMap_Terrain()
{
	DestroyWindow();
}

void CMap_Terrain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, XVertex);
	DDX_Text(pDX, IDC_EDIT4, ZVertex);
	DDX_Text(pDX, IDC_EDIT3, m_dwYMax);
	DDX_Text(pDX, IDC_EDIT9, m_fInterval);
	DDX_Radio(pDX, IDC_RADIO1, m_iType);
	DDX_Text(pDX, IDC_EDIT2, m_szTerrainName);
	DDX_Control(pDX, IDC_EDIT1, m_XVertexBox);
	DDX_Control(pDX, IDC_EDIT4, m_ZVertexBox);
	DDX_Control(pDX, IDC_EDIT9, m_IntervalBox);
	DDX_Control(pDX, IDC_EDIT2, m_TerrainNameBox);
	DDX_Text(pDX, IDC_EDIT8, m_fRange);
	DDX_Radio(pDX, IDC_RADIO3, m_iDirection);
}


BEGIN_MESSAGE_MAP(CMap_Terrain, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMap_Terrain::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMap_Terrain::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMap_Terrain::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CMap_Terrain::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_RADIO1, &CMap_Terrain::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMap_Terrain::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, &CMap_Terrain::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO3, &CMap_Terrain::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO5, &CMap_Terrain::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &CMap_Terrain::OnBnClickedRadio6)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMap_Terrain::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CMap_Terrain 메시지 처리기입니다.
void CMap_Terrain::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return;

	Safe_AddRef(pManagement);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (nullptr == pMainFrame)
			return;

	UpdateData(TRUE);

	if (nullptr == dynamic_cast<CTerrain*>(pMainFrame->GetToolView()->GetTerrainObject()))
	{

		if (FAILED(pManagement->Add_Prototype(0, L"Component_VIBuffer_Terrain", CVIBuffer_Terrain::Create(pMainFrame->GetToolView()->m_pDevice, XVertex, ZVertex, m_fInterval))))
			return;

		if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Terrain", 0, m_szTerrainName)))
		{
			MessageBox(L"GameObject_Terrain Adding Failed", L"System Error", MB_OK);
			return;
		}
		pMainFrame->GetToolView()->SetTerrainTag(m_szTerrainName);

	}
	else
		MessageBox(L"Terrain has existing", L"System Error", MB_OK);

	Safe_Release(pManagement);
	//Safe_Release(pMainFrame);
	UpdateData(FALSE);
}


void CMap_Terrain::OnBnClickedButton3()
{
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	LPDIRECT3DTEXTURE9		pTexture;
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (nullptr == pMainFrame)
		return;
	CManagement* pManagement = CManagement::GetInstance();

	Safe_AddRef(pManagement);

	const CVIBuffer_Terrain* pTerrainBuffer = (const CVIBuffer_Terrain*)pManagement->Get_Component(0, m_szTerrainName, L"Com_VIBuffer");


	LPDIRECT3DVERTEXBUFFER9	pVB = pTerrainBuffer->GetVertexBuffer();
	VTXNORTEX*		pVertices = nullptr;


	D3DXCreateTexture(pMainFrame->GetToolView()->m_pDevice, XVertex, ZVertex, (_uint)m_fInterval, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture);

	D3DLOCKED_RECT			LockRect;

	pTexture->LockRect(0, &LockRect, nullptr, 0);
	pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < XVertex; ++i)
	{
		for (size_t j = 0; j < ZVertex; ++j)
		{
			_uint iIndex = i * XVertex + j;

			((_ulong*)LockRect.pBits)[iIndex] = D3DXCOLOR(pVertices[iIndex].vPosition.y);
		}
	}


	pVB->Unlock();

	pTexture->UnlockRect(0);

	D3DXSaveTextureToFile(L"../Client/Bin/Resource/Texture/Terrain/Terrain.bmp", D3DXIFF_BMP, pTexture, nullptr);

	if (FAILED(SaveVertex()))
		return;

	Safe_Release(pManagement);

	UpdateData(FALSE);
}


void CMap_Terrain::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	dynamic_cast<CTerrain*>(pMainFrame->GetToolView()->GetTerrainObject())->EraseCell();

	UpdateData(FALSE);
}

void CMap_Terrain::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CManagement* pManagement = CManagement::GetInstance();

	Safe_AddRef(pManagement);

	UpdateData(TRUE);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (nullptr == pMainFrame)
		return;
	const CVIBuffer_Terrain* pTerrainBuffer = (const CVIBuffer_Terrain*)pManagement->Get_Component(0, m_szTerrainName, L"Com_VIBuffer");
	
	LPDIRECT3DVERTEXBUFFER9	pVB = pTerrainBuffer->GetVertexBuffer();
	VTXNORTEX*		pVertices = nullptr;

	pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < pTerrainBuffer->GetVerticesNum(); i++)
	{
		if (pVertices[i].vPosition.y >= m_dwYMax)
			pVertices[i].vPosition.y = m_dwYMax;

		else if (pVertices[i].vPosition.y <= (m_dwYMax) * -1.f)
			pVertices[i].vPosition.y = m_dwYMax;
	}
	
	pVB->Unlock();


	CShader* pShader = (CShader*)pManagement->Get_Component(0, m_szTerrainName, L"Com_Shader");
	pShader->Set_Value("g_fBrushRange", &m_fRange, sizeof(float));

	dynamic_cast<CTerrain*>(pMainFrame->GetToolView()->GetTerrainObject())->SetRadius(m_fRange);

	const CTransform* pTerrainTransform = (const CTransform*)pManagement->Get_Component(0, m_szTerrainName, L"Com_Transform");

	//
	Safe_Release(pManagement);
	//Safe_Release(pMainFrame);
	UpdateData(FALSE);
}


BOOL CMap_Terrain::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	XVertex = 129;
	ZVertex = 129;
	m_fInterval = 1.f;
	CString str;
	str.Format(L"%d", XVertex);
	m_XVertexBox.SetWindowTextW(str);

	str.Format(L"%d", ZVertex);
	m_ZVertexBox.SetWindowTextW(str);

	str.Format(L"%f", m_fInterval);
	m_IntervalBox.SetWindowTextW(str);

	m_TerrainNameBox.SetWindowTextW(L"Default Terrain");


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMap_Terrain::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.



	UpdateData(TRUE);
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (nullptr == pMainFrame)
		return;
		
	dynamic_cast<CTerrain*>(pMainFrame->GetToolView()->GetTerrainObject())->SetPassIndex(0);


	UpdateData(FALSE);
}

void CMap_Terrain::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (nullptr == pMainFrame)
		return;

	dynamic_cast<CTerrain*>(pMainFrame->GetToolView()->GetTerrainObject())->SetPassIndex(1);

	//Safe_Release(pMainFrame);
	UpdateData(FALSE);
}

void CMap_Terrain::OnBnClickedRadio3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	dynamic_cast<CTerrain*>(pMainFrame->GetToolView()->GetTerrainObject())->SetIsUp(true);
	UpdateData(FALSE);
}


void CMap_Terrain::OnBnClickedRadio4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	dynamic_cast<CTerrain*>(pMainFrame->GetToolView()->GetTerrainObject())->SetIsUp(false);
	UpdateData(FALSE);
}

void CMap_Terrain::OnBnClickedRadio5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	dynamic_cast<CTerrain*>(pMainFrame->GetToolView()->GetTerrainObject())->SetIsTerrain(true);
	m_bIsEditing = true;
	UpdateData(FALSE);
}


void CMap_Terrain::OnBnClickedRadio6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	dynamic_cast<CTerrain*>(pMainFrame->GetToolView()->GetTerrainObject())->SetIsTerrain(false);
	m_bIsEditing = false;
	UpdateData(FALSE);
}


void CMap_Terrain::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

HRESULT CMap_Terrain::SaveVertex()
{
	
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	vector<_vec3> VB = dynamic_cast<CTerrain*>(pMainFrame->GetToolView()->GetTerrainObject())->GetVertexVector();
	_uint CellNum = dynamic_cast<CTerrain*>(pMainFrame->GetToolView()->GetTerrainObject())->GetCellNum();
	HANDLE			hFile = 0;
	_ulong			dwByte = 0;

	hFile = CreateFile(L"../Client/Bin/Data/Navigation.dat", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_uint iNumCell = CellNum;

	WriteFile(hFile, &iNumCell, sizeof(_uint), &dwByte, nullptr);

	_vec3 vPoint[3];

	for (size_t i = 0; i < iNumCell; i++)
	{
		vPoint[0] =VB.at(i * 3);
		vPoint[1] =VB.at(i * 3 + 1);
		vPoint[2] =VB.at(i * 3 + 2);

		WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);
	}

	CloseHandle(hFile);
	return S_OK;
}