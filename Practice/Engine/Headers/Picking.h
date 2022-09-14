#pragma once

#include "Base.h"

BEGIN(Engine)

class CTransform;
class CPicking final : public CBase
{
private:
	explicit CPicking(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPicking() = default;
public:
	HRESULT Ready_Picking();
	HRESULT Compute_MouseInLocalSpace(const CTransform* pTransform, _vec3* pRay, _vec3* pPivot);
	_bool Intersect_Polygon();
private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
public:
	static CPicking* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

END