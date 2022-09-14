#pragma once

#include "Scene.h"


class CScene_Tool final : public CScene
{
private:
	explicit CScene_Tool(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Tool() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(_double TimeDelta);
	virtual _int LateUpdate_Scene(_double TimeDelta);
	virtual HRESULT Render_Scene();
public:
	static CScene_Tool* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
