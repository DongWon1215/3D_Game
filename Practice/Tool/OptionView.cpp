// OptionView.cpp : 구현 파일입니다.

#include "stdafx.h"
#include "Tool.h"
#include "OptionView.h"

#include "Map_Object.h"
#include "Map_Terrain.h"
#include "Mesh_Animation.h"


// COptionView

IMPLEMENT_DYNCREATE(COptionView, CFormView)

COptionView::COptionView()
	: CFormView(IDD_OPTIONVIEW)
{

}

COptionView::~COptionView()
{
	delete m_pAnimation;
	delete m_pObject;
	delete m_pTerrain;

}

void COptionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tap1);
}

BEGIN_MESSAGE_MAP(COptionView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &COptionView::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// COptionView 진단입니다.

#ifdef _DEBUG
void COptionView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COptionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// COptionView 메시지 처리기입니다.


void COptionView::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_Tap1.GetCurSel();

	switch (CurSel)
	{
	case 0:
		m_pTerrain->ShowWindow(SW_SHOW);
		m_pObject->ShowWindow(SW_HIDE);
		m_pAnimation->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pTerrain->ShowWindow(SW_HIDE);
		m_pObject->ShowWindow(SW_SHOW);
		m_pAnimation->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pTerrain->ShowWindow(SW_HIDE);
		m_pObject->ShowWindow(SW_HIDE);
		m_pAnimation->ShowWindow(SW_SHOW);
		break;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void COptionView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_Tap1.InsertItem(0, L"Terrain Tool");
	m_Tap1.InsertItem(1, L"GameObject Tool");
	m_Tap1.InsertItem(2, L"Animation Tool");
	m_Tap1.SetCurSel(0);

	CRect rect;
	m_Tap1.GetWindowRect(&rect);

	m_pTerrain = new CMap_Terrain;
	m_pTerrain->Create(IDD_MAP_TERRAIN, &m_Tap1);
	m_pTerrain->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pTerrain->ShowWindow(SW_SHOW);

	m_pObject = new CMap_Object;
	m_pObject->Create(IDD_MAP_OBJECT, &m_Tap1);
	m_pObject->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pObject->ShowWindow(SW_HIDE);

	m_pAnimation = new CMesh_Animation;
	m_pAnimation->Create(IDD_MESH_ANIMATION, &m_Tap1);
	m_pAnimation->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pAnimation->ShowWindow(SW_HIDE);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
