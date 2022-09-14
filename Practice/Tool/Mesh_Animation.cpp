// Mesh_Animation.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Mesh_Animation.h"
#include "afxdialogex.h"


// CMesh_Animation 대화 상자입니다.

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


// CMesh_Animation 메시지 처리기입니다.
