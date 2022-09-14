#include "..\Headers\VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pDevice)
	:CVIBuffer(pDevice)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cube::Ready_Component_Prototype()
{
	// CVIBuffer_Cube만의 정점 정보와 갯수, FVF를 셋팅한다.
	m_iStride = sizeof(VTXCUBETEX);
	m_iNumVertices = 8;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);


	// For.Index Buffer
	m_iPolygonIndicesSize = sizeof(INDEX16);
	m_eFormat = D3DFMT_INDEX16;
	m_iNumPrimitive = 12;

	// 셋팅해놓은 정보를 바탕으로해서 정점 배열(버퍼)을 선언했다.
	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	// 정점 배열공간의 앞 주소를 얻어오기위한 포인터.
	VTXCUBETEX*		pVertices = nullptr;

	// 공간을 걸어잠근다.
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// 해당 공간에 값을 채운다.
	pVertices[0].vPosition = _vec3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexUV = pVertices[0].vPosition;

	pVertices[1].vPosition = _vec3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexUV = pVertices[1].vPosition;

	pVertices[2].vPosition = _vec3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexUV = pVertices[2].vPosition;

	pVertices[3].vPosition = _vec3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexUV = pVertices[3].vPosition;

	pVertices[4].vPosition = _vec3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexUV = pVertices[4].vPosition;

	pVertices[5].vPosition = _vec3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexUV = pVertices[5].vPosition;

	pVertices[6].vPosition = _vec3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexUV = pVertices[6].vPosition;

	pVertices[7].vPosition = _vec3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexUV = pVertices[7].vPosition;


	for (size_t i = 0; i < m_iNumVertices; ++i)
		m_pPosition[i] = pVertices[i].vPosition;


	// 공간을 해재하여 정점배열의 값을 사용할 수 있도록 한다.
	m_pVB->Unlock();

	INDEX16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	// +x
	pIndices[0]._1 = 1; pIndices[0]._2 = 5; pIndices[0]._3 = 6;
	pIndices[1]._1 = 1; pIndices[1]._2 = 6; pIndices[1]._3 = 2;

	// -x
	pIndices[2]._1 = 4; pIndices[2]._2 = 0; pIndices[2]._3 = 3;
	pIndices[3]._1 = 4; pIndices[3]._2 = 3; pIndices[3]._3 = 7;

	// +y
	pIndices[4]._1 = 4; pIndices[4]._2 = 5; pIndices[4]._3 = 1;
	pIndices[5]._1 = 4; pIndices[5]._2 = 1; pIndices[5]._3 = 0;

	// -y
	pIndices[6]._1 = 3; pIndices[6]._2 = 2; pIndices[6]._3 = 6;
	pIndices[7]._1 = 3; pIndices[7]._2 = 6; pIndices[7]._3 = 7;

	// +z						 					 	
	pIndices[8]._1 = 5; pIndices[8]._2 = 4; pIndices[8]._3 = 7;
	pIndices[9]._1 = 5; pIndices[9]._2 = 7; pIndices[9]._3 = 6;

	// -z		
	pIndices[10]._1 = 0; pIndices[10]._2 = 1; pIndices[10]._3 = 2;
	pIndices[11]._1 = 0; pIndices[11]._2 = 2; pIndices[11]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Cube::Ready_Component(void * pArg)
{
	return S_OK;
}

_bool CVIBuffer_Cube::Picking(CTransform * pTransform, _vec3 * pPoint)
{
	if (nullptr == m_pPicking)
		return false;

	_vec3 vRay, vPivot;

	if (FAILED(m_pPicking->Compute_MouseInLocalSpace(pTransform, &vRay, &vPivot)))
		return E_FAIL;

	_float fU, fV, fDist;

	_bool bIsColl = false;

	if (bIsColl = D3DXIntersectTri(&m_pPosition[0], &m_pPosition[1], &m_pPosition[2], &vPivot, &vRay, &fU, &fV, &fDist))
	{
		*pPoint = vPivot + *D3DXVec3Normalize(&vRay, &vRay) * fDist;
		return bIsColl;
	}

	if (bIsColl = D3DXIntersectTri(&m_pPosition[0], &m_pPosition[2], &m_pPosition[3], &vPivot, &vRay, &fU, &fV, &fDist))
	{
		*pPoint = vPivot + *D3DXVec3Normalize(&vRay, &vRay) * fDist;
		return bIsColl;
	}

	return _bool();
}

HRESULT CVIBuffer_Cube::Render_VIBuffer()
{
	LPDIRECT3DDEVICE9 pDevice = Get_Graphic_Device();
	if (pDevice == nullptr ||
		m_pVB == nullptr)
		return E_FAIL;

	Safe_AddRef(pDevice);

	pDevice->SetStreamSource(0, m_pVB, 0, m_iStride);
	pDevice->SetFVF(m_dwFVF);
	pDevice->SetIndices(m_pIB);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	Safe_Release(pDevice);

	return S_OK;
}

CVIBuffer_Cube * CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MSG_BOX("Failed while Creating VIBuffer_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Cube::Clone_Component(void * pArg)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed while Creating CVIBuffer_Cube_Copy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	CVIBuffer::Free();
}
