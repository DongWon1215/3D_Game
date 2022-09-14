
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

#include "Management.h"

BEGIN(Engine)
class CPicking;
class CInput;
END

class CToolDoc;
class CToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
private:

public:
	CManagement*		m_pManagement;
	LPDIRECT3DDEVICE9	m_pDevice;
	CRenderer*			m_pRenderer;

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	CGameObject* GetTerrainObject();
	CGameObject* GetGameObject(const _tchar* ObjectTag, _uint iIndex);

public:
	void UpdateInputDevice();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
// �����Դϴ�.
public:
	virtual ~CToolView();

public:
	void SetTerrainTag(const _tchar* pTerrainTag);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HRESULT Ready_ProtoType();

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
private:
	_tchar m_strTerrain[MAX_PATH] = L"";

};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

