// Mesh_Animation.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "Mesh_Animation.h"
#include "afxdialogex.h"


// CMesh_Animation ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMesh_Animation, CDialogEx)

CMesh_Animation::CMesh_Animation(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MESH_ANIMATION, pParent)
{

}

CMesh_Animation::~CMesh_Animation()
{
	DestroyWindow();
}

void CMesh_Animation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMesh_Animation, CDialogEx)
END_MESSAGE_MAP()


// CMesh_Animation �޽��� ó�����Դϴ�.
