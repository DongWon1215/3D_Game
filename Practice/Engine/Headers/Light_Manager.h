#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight;
class CShader;
class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	explicit CLight_Manager();
	virtual ~CLight_Manager() = default;
public:
	D3DLIGHT9 Get_LightDesc(_uint iIndex) const;
public:
	HRESULT Reserve_Light_Manager(_uint iNumLight);
	HRESULT Update_Light_Manager(_double TimeDelta);
	HRESULT Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9* pLightDesc, _double LifeTime);
	HRESULT Render_Light(CShader* pShader);
private:
	vector<CLight*>			m_Lights;
	typedef vector<CLight*>	LIGHTS;
public:
	virtual void Free();
};

END