#pragma once


// CMesh_Animation ��ȭ �����Դϴ�.

class CMesh_Animation : public CDialogEx
{
	DECLARE_DYNAMIC(CMesh_Animation)

public:
	CMesh_Animation(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMesh_Animation();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESH_ANIMATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
