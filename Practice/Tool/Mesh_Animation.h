#pragma once


// CMesh_Animation 대화 상자입니다.

class CMesh_Animation : public CDialogEx
{
	DECLARE_DYNAMIC(CMesh_Animation)

public:
	CMesh_Animation(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMesh_Animation();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESH_ANIMATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
