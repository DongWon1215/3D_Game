#pragma once

// 1. 그려야할 객체들을 그리는 순서에따라 분류하여 모아놓는다.
// 2. 모아놓은 객체들을 순서에따라 그린다.

#include "Component.h"
#include "Target_Manager.h"

BEGIN(Engine)

class CShader;
class CGameObject;
class CPipeLine;
class CLight_Manager;
class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_EFFECT ,RENDER_UI, RENDER_END };
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;
public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);
public:
	HRESULT Add_RenderList(RENDERGROUP eGroup, CGameObject* pGameObject);
	HRESULT Render_RenderList();
public:
	void Set_RenderTarget_Show() { m_bIsShow = !m_bIsShow; }
private:
	list<CGameObject*>			m_RenderList[RENDER_END];
	typedef list<CGameObject*>	RENDERLIST;
private:
	CTarget_Manager*			m_pTarget_Manager = nullptr;
	CLight_Manager*				m_pLight_Manager = nullptr;
private:
	CShader*					m_pShader_LightAcc = nullptr;
	CShader*					m_pShader_Blend = nullptr;
	CVIBuffer_ViewportRect*		m_pBuffer = nullptr;
	CPipeLine*					m_pPipeLine = nullptr;
private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();
private:
	HRESULT Render_Light();
	HRESULT Render_Blend();

	_bool	m_bIsShow = true;

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END