#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TEXTURETYPE { TYPE_GENERAL, TYPE_CUBE, TYPE_END };
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;
public:
	IDirect3DBaseTexture9* Get_Texture(_uint iIndex);
public:
	virtual HRESULT Ready_Component_Prototype(TEXTURETYPE eType, const _tchar* pFilePath, _uint iNumTexture);
	virtual HRESULT Ready_Component(void* pArg);
private:	
	vector<IDirect3DBaseTexture9*>			m_Textures;
	typedef vector<IDirect3DBaseTexture9*>	TEXTURES;
public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURETYPE eType, const _tchar* pFilePath, _uint iNumTexture = 1);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END