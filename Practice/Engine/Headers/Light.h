#pragma once

#include "Base.h"

BEGIN(Engine)

class CShader;
class CVIBuffer_ViewportRect;
class CLight final : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLight() = default;
public:
	D3DLIGHT9 Get_LightDesc() const {
		return m_LightDesc; }
	_double GetLifeTime() const { return m_LifeTime; }
public:
	HRESULT Ready_Light(const D3DLIGHT9* pLightDesc, _double LifeTime);
	HRESULT Update_Light(_double TimeDelta);
	HRESULT Render_Light(CShader* pShader);
private:
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;
	D3DLIGHT9					m_LightDesc;
	CVIBuffer_ViewportRect*		m_pBuffer = nullptr;

private:
	_double m_LifeTime;		//점광원 생존 시간
public:
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9* pLightInfo, _double LifeTime);
	virtual void Free();
};

END