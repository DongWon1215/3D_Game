#include "..\Headers\VIBuffer.h"
#include "..\Headers\Picking.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pVB(nullptr)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_dwFVF(rhs.m_dwFVF)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_pIB(rhs.m_pIB)
	, m_iPolygonIndicesSize(rhs.m_iPolygonIndicesSize)
	, m_eFormat(rhs.m_eFormat)
	, m_pPicking(rhs.m_pPicking)
	, m_pPosition(rhs.m_pPosition)
{
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pIB);
	Safe_AddRef(m_pVB);
}

HRESULT CVIBuffer::Ready_Component_Prototype()
{
	LPDIRECT3DDEVICE9	pGraphic_Device = Get_Graphic_Device();
	if (nullptr == pGraphic_Device)
		return E_FAIL;

	m_pPicking = CPicking::Create(pGraphic_Device);
	if (nullptr == m_pPicking)
		return E_FAIL;

	Safe_AddRef(pGraphic_Device);

	// 정점배열버퍼를 생성한다.
	if (FAILED(pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	m_pPosition = new _vec3[m_iNumVertices];
	ZeroMemory(m_pPosition, sizeof(_vec3) *m_iNumVertices);

	// 인덱스배열버퍼를 생성한다.
	if (FAILED(pGraphic_Device->CreateIndexBuffer(m_iPolygonIndicesSize * m_iNumPrimitive, 0, m_eFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	Safe_Release(pGraphic_Device);

	return NOERROR;
}

HRESULT CVIBuffer::Ready_Component(void * pArg)
{
	return NOERROR;
}

_bool CVIBuffer::Picking(CTransform*	pTransform, _vec3* pPoint)
{

	
	return _bool();
}

void CVIBuffer::Free()
{
	if(false == is_Clone())
		Safe_Delete_Array(m_pPosition);

	Safe_Release(m_pPicking);
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	CComponent::Free();
}
