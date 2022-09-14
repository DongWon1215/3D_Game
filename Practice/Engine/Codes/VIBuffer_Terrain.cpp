#include "..\Headers\VIBuffer_Terrain.h"
#include "../Headers/Management.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_fInterval(rhs.m_fInterval)
	, m_pIndices(rhs.m_pIndices)
	, m_pQuadTree(rhs.m_pQuadTree)
{
	Safe_AddRef(m_pQuadTree);
}

HRESULT CVIBuffer_Terrain::Ready_Component_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ, _float fInterval)
{
	// CVIBuffer_Terrain만의 정점 정보와 갯수, FVF를 셋팅한다.
	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	m_iPolygonIndicesSize = sizeof(INDEX32);
	m_eFormat = D3DFMT_INDEX32;
	m_iNumPrimitive = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	// 셋팅해놓은 정보를 바탕으로해서 정점 배열(버퍼)을 선언했다.
	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	// 정점 배열공간의 앞 주소를 얻어오기위한 포인터.
	VTXNORTEX*		pVertices = nullptr;

	// 공간을 걸어잠근다.
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < iNumVerticesZ; ++i)
	{
		for (size_t j = 0; j < iNumVerticesX; ++j)
		{
			_uint iIndex = i * iNumVerticesX + j;

			pVertices[iIndex].vPosition = _vec3(j * fInterval, 0.f, i * fInterval);
			pVertices[iIndex].vNormal = _vec3(0.f, 1.f, 0.f);
			pVertices[iIndex].vTexUV = _vec2(j / float(iNumVerticesX - 1), i / float(iNumVerticesZ - 1));
		}
	}

	// 공간을 해재하여 정점배열의 값을 사용할 수 있도록 한다.


	INDEX32*	pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint		iNumPolygon = 0;

	for (size_t i = 0; i < iNumVerticesZ - 1; ++i)
	{
		for (size_t j = 0; j < iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * iNumVerticesX + j;

			pIndices[iNumPolygon]._1 = iIndex + iNumVerticesX;
			pIndices[iNumPolygon]._2 = iIndex + iNumVerticesX + 1;
			pIndices[iNumPolygon]._3 = iIndex + 1;
			++iNumPolygon;

			pIndices[iNumPolygon]._1 = iIndex + iNumVerticesX;
			pIndices[iNumPolygon]._2 = iIndex + 1;
			pIndices[iNumPolygon]._3 = iIndex;
			++iNumPolygon;
		}
	}

	m_pIB->Unlock();


	for (size_t i = 0; i < m_iNumVertices; ++i)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

	for (size_t i = 0; i < m_iNumVertices; ++i)
		m_pPosition[i] = pVertices[i].vPosition;

	m_pVB->Unlock();

	return NOERROR;
}

HRESULT CVIBuffer_Terrain::Ready_Component_Prototype(const _tchar * pHeightMapPath, _float fInterval)
{
	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(pHeightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	BITMAPFILEHEADER		fh;
	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

	BITMAPINFOHEADER		ih;
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	_ulong*		pPixel = new _ulong[ih.biWidth * ih.biHeight];

	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, 0);

	CloseHandle(hFile);

	// CVIBuffer_Terrain만의 정점 정보와 갯수, FVF를 셋팅한다.
	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = ih.biWidth * ih.biHeight;
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	m_fInterval = fInterval;

	m_iPolygonIndicesSize = sizeof(INDEX32);
	m_eFormat = D3DFMT_INDEX32;
	m_iNumPrimitive = (ih.biWidth - 1) * (ih.biHeight - 1) * 2;

	// 셋팅해놓은 정보를 바탕으로해서 정점 배열(버퍼)을 선언했다.
	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	// 정점 배열공간의 앞 주소를 얻어오기위한 포인터.
	VTXNORTEX*		pVertices = nullptr;

	// 공간을 걸어잠근다.
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < ih.biHeight; ++i)
	{
		for (size_t j = 0; j < ih.biWidth; ++j)
		{
			_uint iIndex = i * ih.biWidth + j;

			pVertices[iIndex].vPosition = _vec3(j * fInterval, /*0.0f*/(pPixel[iIndex] & 0x000000ff) / 10.0f, i * fInterval);
			pVertices[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _vec2(j / float(ih.biWidth - 1), i / float(ih.biHeight - 1));
		}
	}

	m_pIndices = new INDEX32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(INDEX32) * m_iNumPrimitive);

	// 공간을 해재하여 정점배열의 값을 사용할 수 있도록 한다.
	INDEX32*	pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint		iNumPolygon = 0;

	for (size_t i = 0; i < ih.biHeight - 1; ++i)
	{
		for (size_t j = 0; j < ih.biWidth - 1; ++j)
		{
			_uint iIndex = i * ih.biWidth + j;

			_vec3		vSour, vDest, vNormal;

			pIndices[iNumPolygon]._1 = iIndex + ih.biWidth;
			pIndices[iNumPolygon]._2 = iIndex + ih.biWidth + 1;
			pIndices[iNumPolygon]._3 = iIndex + 1;

			vSour = pVertices[pIndices[iNumPolygon]._2].vPosition - pVertices[pIndices[iNumPolygon]._1].vPosition;
			vDest = pVertices[pIndices[iNumPolygon]._3].vPosition - pVertices[pIndices[iNumPolygon]._2].vPosition;
			D3DXVec3Cross(&vNormal, &vSour, &vDest);

			pVertices[pIndices[iNumPolygon]._1].vNormal += *D3DXVec3Normalize(&vNormal, &vNormal);
			pVertices[pIndices[iNumPolygon]._2].vNormal += *D3DXVec3Normalize(&vNormal, &vNormal);
			pVertices[pIndices[iNumPolygon]._3].vNormal += *D3DXVec3Normalize(&vNormal, &vNormal);
			++iNumPolygon;



			pIndices[iNumPolygon]._1 = iIndex + ih.biWidth;
			pIndices[iNumPolygon]._2 = iIndex + 1;
			pIndices[iNumPolygon]._3 = iIndex;

			vSour = pVertices[pIndices[iNumPolygon]._2].vPosition - pVertices[pIndices[iNumPolygon]._1].vPosition;
			vDest = pVertices[pIndices[iNumPolygon]._3].vPosition - pVertices[pIndices[iNumPolygon]._2].vPosition;
			D3DXVec3Cross(&vNormal, &vSour, &vDest);

			pVertices[pIndices[iNumPolygon]._1].vNormal += *D3DXVec3Normalize(&vNormal, &vNormal);
			pVertices[pIndices[iNumPolygon]._2].vNormal += *D3DXVec3Normalize(&vNormal, &vNormal);
			pVertices[pIndices[iNumPolygon]._3].vNormal += *D3DXVec3Normalize(&vNormal, &vNormal);
			++iNumPolygon;
		}
	}

	m_pIB->Unlock();

	for (size_t i = 0; i < m_iNumVertices; ++i)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

	for (size_t i = 0; i < m_iNumVertices; ++i)
		m_pPosition[i] = pVertices[i].vPosition;

	m_pVB->Unlock();

	Safe_Delete_Array(pPixel);

	m_pQuadTree = CQuadTree::Create(Get_Graphic_Device(), m_pPosition, m_iNumVerticesX, m_iNumVerticesZ);
	if (nullptr == m_pQuadTree)
		return E_FAIL;

	return NOERROR;
}

HRESULT CVIBuffer_Terrain::Ready_Component(void * pArg)
{
	return NOERROR;
}

_bool CVIBuffer_Terrain::Picking(const CTransform * pTransform, _vec3 * pPoint) const
{
	if (nullptr == m_pPicking)
		return false;

	_vec3		vRay, vPivot;

	if (FAILED(m_pPicking->Compute_MouseInLocalSpace(pTransform, &vRay, &vPivot)))
		return false;

	_float		fU, fV, fDist;

	_bool		isColl = false;

	for (size_t i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			if (isColl = D3DXIntersectTri(&m_pPosition[iIndices[1]], &m_pPosition[iIndices[0]], &m_pPosition[iIndices[2]], &vPivot, &vRay, &fU, &fV, &fDist))
			{
				_vec3		vWidth = (m_pPosition[iIndices[0]] - m_pPosition[iIndices[1]]);

				_float		fLength = D3DXVec3Length(&vWidth);

				D3DXVec3Normalize(&vWidth, &vWidth);

				vWidth *= (fLength * fU);

				_vec3		vDepth = (m_pPosition[iIndices[2]] - m_pPosition[iIndices[1]]);

				fLength = D3DXVec3Length(&vDepth);

				D3DXVec3Normalize(&vDepth, &vDepth);

				vDepth *= (fLength * fV);

				*pPoint = m_pPosition[iIndices[1]] + vWidth + vDepth;

				//*pPoint = (vPivot + fDist * vRay);

				return isColl;
			}


			if (isColl = D3DXIntersectTri(&m_pPosition[iIndices[3]], &m_pPosition[iIndices[2]], &m_pPosition[iIndices[0]], &vPivot, &vRay, &fU, &fV, &fDist))
			{
				_vec3		vWidth = (m_pPosition[iIndices[2]] - m_pPosition[iIndices[3]]);

				_float		fLength = D3DXVec3Length(&vWidth);

				D3DXVec3Normalize(&vWidth, &vWidth);

				vWidth *= (fLength * fU);

				_vec3		vDepth = (m_pPosition[iIndices[0]] - m_pPosition[iIndices[3]]);

				fLength = D3DXVec3Length(&vDepth);

				D3DXVec3Normalize(&vDepth, &vDepth);

				vDepth *= (fLength * fV);

				*pPoint = m_pPosition[iIndices[3]] + vWidth + vDepth;
				//*pPoint = (vPivot + fDist * vRay);


				return isColl;
			}
		}
	}
	return _bool(isColl);
}

HRESULT CVIBuffer_Terrain::Culling(CTransform* pTransform, CFrustum * pFrustum)
{
	ZeroMemory(m_pIndices, sizeof(INDEX32) * (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2);

	if (FAILED(pFrustum->Transform_ToLocal(pTransform)))
		return E_FAIL;

	_uint iNumPolygons = 0;

	m_pQuadTree->Culling_ToQuadTree(pFrustum, m_pPosition, m_pIndices, &iNumPolygons);
	m_iNumPrimitive = iNumPolygons;

	// 화면에 그려야할 폴리곤의 인덱스들.
	INDEX32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	memcpy(pIndices, m_pIndices, sizeof(INDEX32) * m_iNumPrimitive);

	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CVIBuffer_Terrain::Render_VIBuffer()
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

_float CVIBuffer_Terrain::Compute_Y(_vec3 vPosition) const
{
	_uint	iIndex = _uint(vPosition.z / m_fInterval) * m_iNumVerticesX + _uint(vPosition.x / m_fInterval);

	_float	fRatioX = (vPosition.x - m_pPosition[iIndex + m_iNumVerticesX].x) / m_fInterval;
	_float	fRatioZ = (m_pPosition[iIndex + m_iNumVerticesX].z - vPosition.z) / m_fInterval;

	_float	fY[4] = {
		m_pPosition[iIndex + m_iNumVerticesX].y,
		m_pPosition[iIndex + m_iNumVerticesX + 1].y,
		m_pPosition[iIndex + 1].y,
		m_pPosition[iIndex].y
	};

	if (fRatioX > fRatioZ) // 오른쪽위에 있었어요.
	{
		return fY[0] + (fY[1] - fY[0]) * fRatioX + (fY[2] - fY[1]) * fRatioZ;
	}
	else // 왼쪽 하단에 있었어요.
	{
		return fY[0] + (fY[3] - fY[0]) * fRatioZ + (fY[2] - fY[3]) * fRatioX;
	}
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ, _float fInterval)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype(iNumVerticesX, iNumVerticesZ, fInterval)))
	{
		MSG_BOX("Failed while Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pHeightMapPath, _float fInterval)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype(pHeightMapPath, fInterval)))
	{
		MSG_BOX("Failed while Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone_Component(void * pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed while Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	if (false == is_Clone())
		Safe_Delete_Array(m_pIndices);

	Safe_Release(m_pQuadTree);

	CVIBuffer::Free();
}
