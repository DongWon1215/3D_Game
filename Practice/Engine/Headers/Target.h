#pragma once

#include "Base.h"
#include "VIBuffer_ViewportRect.h"

BEGIN(Engine)

class CShader;
class CTarget final : public CBase
{
private:
	explicit CTarget(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTarget() = default;
public:	
	HRESULT Ready_Target(_uint iWidth, _uint iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor);
	HRESULT SetUp_OnGraphicDev(_uint iIndex);
	HRESULT SetUp_OnShader(CShader* pShader, D3DXHANDLE ConstantName);
	HRESULT Release_OnGraphicDev(_uint iIndex);
	HRESULT Clear_Target();
	
#ifdef _DEBUG
public:
	HRESULT Ready_Debug_Buffer(_float fStartX, _float fStartY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer();
#endif // _DEBUG
private:
	LPDIRECT3DDEVICE9			m_pDevice = nullptr;
	LPDIRECT3DTEXTURE9			m_pTarget_Texture = nullptr;
	LPDIRECT3DSURFACE9			m_pTarget_Surface = nullptr;
	LPDIRECT3DSURFACE9			m_pOld_Surface = nullptr;
private:
	D3DXCOLOR		m_ClearColor;
#ifdef _DEBUG
private:
	CVIBuffer_ViewportRect*		m_pDebugBuffer = nullptr;

#endif // _DEBUG
public:
	static CTarget* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWidth, _uint iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor);
	virtual void Free();
};

END