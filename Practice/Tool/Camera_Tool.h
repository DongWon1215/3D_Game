#pragma once

#include "Camera.h"

class CToolView;
class CCamera_Tool final : public CCamera
{
private:
	explicit CCamera_Tool(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Tool(const CCamera_Tool& rhs);
	virtual ~CCamera_Tool() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();
public:
	static CCamera_Tool* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();

private:
	CToolView* m_pToolView;


};

