#pragma once

#include "Component.h"
#include "Transform.h"
#include "Picking.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;
public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);

public:
	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() const {
		return m_pVB;
	}
	_uint GetVerticesNum() const {
		return m_iNumVertices;
	}

public:
	// ���� �ͷ��ΰ� �浹�ϴ� ������ Ȯ�ν����ֱ� ����
	virtual _bool Picking(CTransform*	pTransform, _vec3* pPoint);
protected:
	// �������� ���鶧 ����ϰ� �� ���ؽ��� ����
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	_uint						m_iStride = 0;
	_uint						m_iNumVertices = 0;
	_ulong						m_dwFVF = 0;
protected:
	// �������� ���� �� ����ϰ� �� ������ ������ �����ϴ� ����
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
	D3DFORMAT					m_eFormat;
	_uint						m_iPolygonIndicesSize = 0;
	_uint						m_iNumPrimitive = 0;
protected:
	CPicking*					m_pPicking = nullptr;
	_vec3*						m_pPosition = nullptr;
public:
	virtual CComponent* Clone_Component(void* pArg) = 0;
	virtual void Free();
};

END