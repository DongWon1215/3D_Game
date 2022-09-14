#pragma once

// 절두체의 정보를 가진다.(월드)
#include "Component.h"
#include "Transform.h"
#include "PipeLine.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum final : public CComponent
{
private:
	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CFrustum() = default;
public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);
public:
	_bool Culling_ToFrustum(CTransform* pTransform, _float fRadius = 0.f);
	HRESULT Transform_ToLocal(CTransform* pTransform);
	_bool is_InFrustum(_vec3 vPointInLocal, _float fRadius);
private:
	_vec3			m_vProjPoint[8];
	CPipeLine*		m_pPipeLine = nullptr;
	D3DXPLANE		m_Plane[6];
public:
	static CFrustum* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END