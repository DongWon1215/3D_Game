#include "..\Headers\VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{

}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Rect::Ready_Component_Prototype()
{
	// CVIBuffer_Rect���� ���� ������ ����, FVF�� �����Ѵ�.
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	

	// For.Index Buffer
	m_iPolygonIndicesSize = sizeof(INDEX16);
	m_eFormat = D3DFMT_INDEX16;
	m_iNumPrimitive = 2;

	// �����س��� ������ ���������ؼ� ���� �迭(����)�� �����ߴ�.
	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	// ���� �迭������ �� �ּҸ� ���������� ������.
	VTXTEX*		pVertices = nullptr;

	// ������ �ɾ���ٴ�.
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// �ش� ������ ���� ä���.
	pVertices[0].vPosition = _vec3(-0.5f, 0.5f, 0.0f);
	pVertices[0].vTexUV = _vec2(0.0f, 0.f);

	pVertices[1].vPosition = _vec3(0.5f, 0.5f, 0.0f);
	pVertices[1].vTexUV = _vec2(1.0f, 0.f);

	pVertices[2].vPosition = _vec3(0.5f, -0.5f, 0.0f);
	pVertices[2].vTexUV = _vec2(1.0f, 1.f);

	pVertices[3].vPosition = _vec3(-0.5f, -0.5f, 0.0f);
	pVertices[3].vTexUV = _vec2(0.f, 1.f);

	for (size_t i = 0; i < m_iNumVertices; ++i)	
		m_pPosition[i] = pVertices[i].vPosition;
	

	// ������ �����Ͽ� �����迭�� ���� ����� �� �ֵ��� �Ѵ�.
	m_pVB->Unlock();

	INDEX16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;	

	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CVIBuffer_Rect::Ready_Component(void * pArg)
{
	return NOERROR;
}

_bool CVIBuffer_Rect::Picking(CTransform*	pTransform, _vec3 * pPoint)
{
	if (nullptr == m_pPicking)
		return false;

	_vec3		vRay, vPivot;

	if (FAILED(m_pPicking->Compute_MouseInLocalSpace(pTransform, &vRay, &vPivot)))
		return false;

	_float		fU, fV, fDist;

	_bool		isColl = false;

	if (isColl = D3DXIntersectTri(&m_pPosition[0], &m_pPosition[1], &m_pPosition[2], &vPivot, &vRay, &fU, &fV, &fDist))
	{
		*pPoint = vPivot + *D3DXVec3Normalize(&vRay, &vRay) * fDist;
		return isColl;
	}


	if (isColl = D3DXIntersectTri(&m_pPosition[0], &m_pPosition[2], &m_pPosition[3], &vPivot, &vRay, &fU, &fV, &fDist))
	{
		*pPoint = vPivot + *D3DXVec3Normalize(&vRay, &vRay) * fDist;
		return isColl;
	}

	return _bool(isColl);
}

HRESULT CVIBuffer_Rect::Render_VIBuffer()
{
	LPDIRECT3DDEVICE9	pDevice = Get_Graphic_Device();	

	if (nullptr == pDevice || 
		nullptr == m_pVB)
		return E_FAIL;

	Safe_AddRef(pDevice);

	pDevice->SetStreamSource(0, m_pVB, 0, m_iStride);
	pDevice->SetFVF(m_dwFVF);
	pDevice->SetIndices(m_pIB);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	Safe_Release(pDevice);

	return NOERROR;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MSG_BOX("Failed while Creating CVIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Rect::Clone_Component(void * pArg)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed while Creating CVIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	CVIBuffer::Free();
}
