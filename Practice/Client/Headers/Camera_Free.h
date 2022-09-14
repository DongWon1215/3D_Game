#pragma once

#include "Client_Extend.h"
#include "Camera.h"


class CCamera_Free : public CCamera
{
private:
	explicit CCamera_Free(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	HRESULT Late_Ready_GameObject();
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:	// 레이 확인후 지울것
	_matrix GetProjMat() const {return m_ProjMatrix;}
	_matrix GetViewMat() const {return m_ViewMatrix;}
	
	_vec3 GetState(CTransform::STATE eState) const { return m_pTransformCom->Get_State(eState); }

	CTransform* GetTransform() const { return m_pTransformCom; }

	HRESULT Camera_Shaking();

private:
	_matrix*	m_pViewMatrix = nullptr;
	_matrix*	m_pHeadMatrix = nullptr;
	_matrix*	m_pPlayerMatrix = nullptr;
	_bool		m_bIsReady = false;
public:
	static CCamera_Free* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();

};
