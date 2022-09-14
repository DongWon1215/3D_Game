#pragma once

#include "VIBuffer.h"
#include "Transform.h"
#include "Frustum.h"
#include "QuadTree.h"



BEGIN(Engine)


class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	explicit CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;
public:
	virtual HRESULT Ready_Component_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ, _float fInterval);
	virtual HRESULT Ready_Component_Prototype(const _tchar* pHeightMapPath, _float fInterval);
	virtual HRESULT Ready_Component(void* pArg);
	virtual _bool Picking(const CTransform*	pTransform, _vec3* pPoint) const;
	HRESULT Culling(CTransform* pTransform, CFrustum* pFrustum);
public:
	HRESULT Render_VIBuffer();
	_float Compute_Y(_vec3	vPosition)const;
private:
	CQuadTree*		m_pQuadTree = nullptr;
	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;
	_float			m_fInterval = 0.f;
	INDEX32*		m_pIndices = nullptr;

public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ, _float fInterval);
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMapPath, _float fInterval);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END