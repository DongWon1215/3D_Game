#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract : public CBase
{
protected:	
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene() = default;

public: // For.Getter
	LPDIRECT3DDEVICE9 GetDevice() const {
		return m_pDevice; }
public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(_double TimeDelta);
	virtual _int LateUpdate_Scene(_double TimeDelta);
	virtual HRESULT Render_Scene();
private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
public:
	virtual void Free();
};

END