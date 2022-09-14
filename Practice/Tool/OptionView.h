#pragma once
#include "afxcmn.h"



// COptionView �� ���Դϴ�.
class CMap_Terrain;
class CMap_Object;
class CMesh_Animation;
class COptionView : public CFormView
{
	DECLARE_DYNCREATE(COptionView)

protected:
	COptionView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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


