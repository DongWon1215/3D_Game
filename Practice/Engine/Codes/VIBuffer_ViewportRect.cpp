#include "..\Headers\VIBuffer_ViewportRect.h"

CVIBuffer_ViewportRect::CVIBuffer_ViewportRect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_ViewportRect::CVIBuffer_ViewportRect(const CVIBuffer_ViewportRect & rhs)
	: CVIBuffer(rhs)
	, m_pDeclaration(rhs.m_pDeclaration)
{
	Safe_AddRef(m_pDeclaration);
}

HRESULT CVIBuffer_ViewportRect::Ready_Component_Prototype(_float fStartX, _float fStartY, _float fSizeX, _float fSizeY)
{

	m_iStride = sizeof(VTXVIEWPORTTEX);
	m_iNumVertices = 4;
	m_dwFVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

	// For.Index Buffer
	m_iPolygonIndicesSize = sizeof(INDEX16);
	m_eFormat = D3DFMT_INDEX16;
	m_iNumPrimitive = 2;

	// 셋팅해놓은 정보를 바탕으로해서 정점 배열(버퍼)을 선언했다.
	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	// 정점 배열공간의 앞 주소를 얻어오기위한 포인터.
	VTXVIEWPORTTEX*		pVertices = nullptr;

	// 공간을 걸어잠근다.
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// 해당 공간에 값을 채운다.
	pVertices[0].vPosition = _vec4(fStartX, fStartY, 0.0f, 1.f);
	pVertices[0].vTexUV = _vec2(0.0f, 0.f);

	pVertices[1].vPosition = _vec4(fStartX + fSizeX, fStartY, 0.0f, 1.f);
	pVertices[1].vTexUV = _vec2(1.0f, 0.f);

	pVertices[2].vPosition = _vec4(fStartX + fSizeX, fStartY + fSizeY, 0.0f, 1.f);
	pVertices[2].vTexUV = _vec2(1.0f, 1.f);

	pVertices[3].vPosition = _vec4(fStartX, fStartY + fSizeY, 0.0f, 1.f);
	pVertices[3].vTexUV = _vec2(0.f, 1.f);

	//for (size_t i = 0; i < m_iNumVertices; ++i)	
	//	m_pPosition[i] = pVertices[i].vPosition;


	// 공간을 해재하여 정점배열의 값을 사용할 수 있도록 한다.
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

HRESULT CVIBuffer_ViewportRect::Ready_Component(void * pArg)
{
	return NOERROR;
}

HRESULT CVIBuffer_ViewportRect::Render_VIBuffer()
{
	LPDIRECT3DDEVICE9	pGraphic_Device = Get_Graphic_Device();

	if (nullptr == pGraphic_Device ||
		nullptr == m_pVB)
		return E_FAIL;

	Safe_AddRef(pGraphic_Device);

	pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	pGraphic_Device->SetFVF(m_dwFVF);
	pGraphic_Device->SetIndices(m_pIB);
	pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	Safe_Release(pGraphic_Device);

	return NOERROR;
}

CVIBuffer_ViewportRect * CVIBuffer_ViewportRect::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fStartX, _float fStartY, _float fSizeX, _float fSizeY)
{
	CVIBuffer_ViewportRect* pInstance = new CVIBuffer_ViewportRect(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype(fStartX, fStartY, fSizeX, fSizeY)))
	{
		MSG_BOX("Failed while Creating CVIBuffer_ViewportRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_ViewportRect::Clone_Component(void * pArg)
{
	CVIBuffer_ViewportRect* pInstance = new CVIBuffer_ViewportRect(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed while Creating CVIBuffer_ViewportRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_ViewportRect::Free()
{
	Safe_Release(m_pDeclaration);

	CVIBuffer::Free();
}
