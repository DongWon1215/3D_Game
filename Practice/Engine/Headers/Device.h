#pragma once

#include "Base.h"

BEGIN(Engine)

class CDevice final : public CBase
{
	DECLARE_SINGLETON(CDevice)
public:
	enum MODE { MODE_FULL, MODE_WIN };
private:
	explicit CDevice();
	virtual ~CDevice() = default;
public:
	LPDIRECT3DDEVICE9 Get_GraphicDev() {
		return m_pDevice; }	
public:
	HRESULT Ready_Device(HWND hWnd, MODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device);
private:	
	LPDIRECT3D9			m_pSDK = nullptr;	
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;
public:
	virtual void Free();	
};

END