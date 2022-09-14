
// ToolView.h : CToolView 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
private:

public:
	CManagement*		m_pManagement;
	LPDIRECT3DDEVICE9	m_pDevice;
	CRenderer*			m_pRenderer;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	CGameObject* GetTerrainObject();
	CGameObject* GetGameObject(const _tchar* ObjectTag, _uint iIndex);

public:
	void UpdateInputDevice();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
// 구현입니다.
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

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
private:
	_tchar m_strTerrain[MAX_PATH] = L"";

};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

