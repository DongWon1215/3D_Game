#pragma once
#include "afxwin.h"

// CMap_Terrain 대화 상자입니다.

class CMainFrame;
class CToolView;
class CMap_Terrain : public CDialogEx
{
	DECLARE_DYNAMIC(CMap_Terrain)

public:
	CMap_Terrain(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMap_Terrain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAP_TERRAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	UINT XVertex;
	UINT ZVertex;
	int m_iType;
	int m_iDirection;
	long m_MouseX;
	long m_MouseY;
	long m_MouseZ;
	float m_dwYMax;
	float m_fRange;
	float m_fInterval;
	bool m_bIsEditing;

public:
	CListBox m_TerrainBox;
	CString m_szTerrainName;
	CEdit m_XVertexBox;
	CEdit m_ZVertexBox;
	CEdit m_IntervalBox;
	CEdit m_TerrainNameBox;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio3();

public:
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnLbnSelchangeList1();

public:
	HRESULT SaveVertex();


};
