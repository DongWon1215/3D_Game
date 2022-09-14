#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Management.h"

// CMap_Object ��ȭ �����Դϴ�.

class CToolView;
class CObject_Tool;
class CMap_Object : public CDialogEx
{
	DECLARE_DYNAMIC(CMap_Object)

public:
	typedef struct TagObject
	{
		const _tchar* FileName;			//�޽� �̸�, �˻���, ���� Ű��
		const _tchar* ProtoTypeName;	//������ Ÿ�� ������Ʈ �̸�
		const _tchar* ObjectName;		//������Ʈ ���� �̸�
	}ProtoType;

public:
	typedef struct tagObject
	{
		D3DXMATRIX WorldMatrix;
		ProtoType TextInfo;
	}OBJECT;

public:
	CMap_Object(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMap_Object();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAP_OBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit11();

public:
	int m_Angle;
	float m_dwScaleX;
	float m_dwScaleY;
	float m_dwScaleZ;

	TCHAR szMeshName[MAX_PATH];

	virtual BOOL OnInitDialog();

public:
	CEdit AngleBox;
	CSliderCtrl m_AngleControl;
	D3DXVECTOR3 m_vecScale;
	CListBox m_ListBox;

public:
	_vec3 m_vPosition;

	_matrix m_matLocal;

public:
	vector<OBJECT> m_vecObject;

	unordered_map<const _tchar*, ProtoType> m_umObject;
	typedef unordered_map <const _tchar*, ProtoType> ProtoTypeMAP;


public:
	void SetProtoType();
	long m_dwObjectXPosition;
	long m_dwObjectYPosition;
	long m_dwObjectZPosition;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
};
