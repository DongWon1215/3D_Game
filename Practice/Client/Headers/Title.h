#pragma once

#include "Client_Extend.h"
#include "Scene.h"
#include "Loading.h"

BEGIN(Client)

class CTitle final : public CScene
{
private:
	explicit CTitle(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTitle() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(_double TimeDelta);
	virtual _int LateUpdate_Scene(_double TimeDelta);
	virtual HRESULT Render_Scene();
private:
	CLoading*		m_pLoading = nullptr;
private:
	HRESULT Ready_GameObject_Prototype();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
public:
	static CTitle* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free();
};

END
