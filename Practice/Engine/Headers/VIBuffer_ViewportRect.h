#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_ViewportRect final : public CVIBuffer
{
private:
	explicit CVIBuffer_ViewportRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_ViewportRect(const CVIBuffer_ViewportRect& rhs);
	virtual ~CVIBuffer_ViewportRect() = default;
public:
	virtual HRESULT Ready_Component_Prototype(_float fStartX, _float fStartY, _float fSizeX, _float fSizeY);
	virtual HRESULT Ready_Component(void* pArg);	
public:
	HRESULT Render_VIBuffer();
private:
	LPDIRECT3DVERTEXDECLARATION9			m_pDeclaration = nullptr;	
public:
	static CVIBuffer_ViewportRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fStartX, _float fStartY, _float fSizeX, _float fSizeY);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END