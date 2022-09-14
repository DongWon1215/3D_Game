#pragma once
#include "afxcmn.h"



// COptionView 폼 뷰입니다.
class CMap_Terrain;
class CMap_Object;
class CMesh_Animation;
class COptionView : public CFormView
{
	DECLARE_DYNCREATE(COptionView)

protected:
	COptionView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~COptionView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIONVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_Tap1;

public:
	CMap_Object* m_pObject;
	CMap_Terrain* m_pTerrain;
	CMesh_Animation* m_pAnimation;
	virtual void OnInitialUpdate();
};


