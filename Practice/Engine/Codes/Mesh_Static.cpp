#include "..\Headers\Mesh_Static.h"

CMesh_Static::CMesh_Static(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{

}

CMesh_Static::CMesh_Static(const CMesh_Static & rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacencyBuffer(rhs.m_pAdjacencyBuffer)
	, m_pMaterialsBuffer(rhs.m_pMaterialsBuffer)
	, m_dwNumMaterials(rhs.m_dwNumMaterials)
	, m_pMaterials(rhs.m_pMaterials)
	, m_pMeshTexture(rhs.m_pMeshTexture)
{
	Safe_AddRef(m_pMesh);
	Safe_AddRef(m_pAdjacencyBuffer);
	Safe_AddRef(m_pMaterialsBuffer);

	for (size_t i = 0; i < m_dwNumMaterials; ++i)
	{
		for (size_t j = 0; j < MESHTEXTURE::TYPE_END; ++j)
		{
			Safe_AddRef(m_pMeshTexture[i].pTexture[j]);
		}		
	}
}

LPDIRECT3DTEXTURE9 CMesh_Static::Get_MaterialTexture(_ulong dwSubSetIndex, MESHTEXTURE::TYPE eType) const
{
	if (m_dwNumMaterials <= dwSubSetIndex || 
		nullptr == m_pMeshTexture)
		return nullptr;

	return m_pMeshTexture[dwSubSetIndex].pTexture[eType];	
}

HRESULT CMesh_Static::Ready_Component_Prototype(const _tchar* pFilePath, const _tchar* pFileName, const _matrix* pLocalMatrix)
{
	_tchar		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	if (FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, Get_Graphic_Device(), 
		&m_pAdjacencyBuffer, &m_pMaterialsBuffer, nullptr, &m_dwNumMaterials, &m_pMesh)))
		return E_FAIL;

	// 사용할 수 있는 재질정보를 셋팅해놓는다.
	m_pMaterials = new D3DXMATERIAL[m_dwNumMaterials];
	memcpy(m_pMaterials, m_pMaterialsBuffer->GetBufferPointer(), sizeof(D3DXMATERIAL) * m_dwNumMaterials);

	m_pMeshTexture = new MESHTEXTURE[m_dwNumMaterials];
	ZeroMemory(m_pMeshTexture, sizeof(MESHTEXTURE) * m_dwNumMaterials);

	for (size_t i = 0; i < m_dwNumMaterials; ++i)
	{
		lstrcpy(szFullPath, pFilePath);

		_tchar		szFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_ACP, 0, m_pMaterials[i].pTextureFilename, strlen(m_pMaterials[i].pTextureFilename)
			, szFileName, MAX_PATH);

		lstrcat(szFullPath, szFileName);

		_tchar		szOut[MAX_PATH] = L"";

		// For.Diffuse_Map
		D3DXCreateTextureFromFile(Get_Graphic_Device(), szFullPath, &m_pMeshTexture[i].pTexture[MESHTEXTURE::TYPE_DIFFUSE]);
		
		// For.Normal_Map	
		if (!FAILED(Change_TextureFileName(szOut, szFullPath, L"D", L"N")))
		D3DXCreateTextureFromFile(Get_Graphic_Device(), szOut, &m_pMeshTexture[i].pTexture[MESHTEXTURE::TYPE_NORMAL]);
		
		// For.Specular_Map
		if (!FAILED(Change_TextureFileName(szOut, szFullPath, L"D", L"S")))
		D3DXCreateTextureFromFile(Get_Graphic_Device(), szOut, &m_pMeshTexture[i].pTexture[MESHTEXTURE::TYPE_SPECULAR]);

		// For.Emissive_Map
		if (!FAILED(Change_TextureFileName(szOut, szFullPath, L"D", L"E")))
		D3DXCreateTextureFromFile(Get_Graphic_Device(), szOut, &m_pMeshTexture[i].pTexture[MESHTEXTURE::TYPE_EMISSIVE]);
	}

	void*	pVertices = nullptr;
	
	m_pMesh->LockVertexBuffer(0, &pVertices);

	_ulong dwNumVertices = m_pMesh->GetNumVertices();

	_uint iStride = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 메시를 구성하는 정점의 정의를 가져온다.
	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];

	m_pMesh->GetDeclaration(Decl);

	_ushort NormalOffset = 0;

	for (size_t i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if(Decl[i].Usage == D3DDECLUSAGE_NORMAL)
		{
			NormalOffset = Decl[i].Offset;
			break;
		}
	}



	

	for (_ulong i = 0; i < dwNumVertices; ++i)
	{
		// 정점의 위치벡터를 LocalMatrix만큼 변환시킨다.
		D3DXVec3TransformCoord((_vec3*)((_byte*)pVertices + (i * iStride)), (_vec3*)((_byte*)pVertices + (i * iStride)), pLocalMatrix);
		D3DXVec3TransformNormal((_vec3*)((_byte*)pVertices + NormalOffset + (i * iStride)), (_vec3*)((_byte*)pVertices + NormalOffset + (i * iStride)), pLocalMatrix);
	}		

	m_pMesh->UnlockVertexBuffer();

	return NOERROR;
}

HRESULT CMesh_Static::Ready_Component(void * pArg)
{
	return NOERROR;
}

HRESULT CMesh_Static::Render_Mesh(_uint iSubsetIndex)
{
	if (nullptr == m_pMesh)
		return E_FAIL;

	m_pMesh->DrawSubset(iSubsetIndex);

	return NOERROR;
}

HRESULT CMesh_Static::Change_TextureFileName(_tchar* pOut, _tchar* pIn, const _tchar* pSourText, const _tchar* pDestText)
{
	_uint iLength = lstrlen(pIn);

	lstrcpy(pOut, pIn);

	for (size_t i = iLength - 4; i > 0; --i)
	{		
		if (pIn[i] == *pSourText)
		{
			pOut[i] = *pDestText;
			return S_OK;
		}
	}

	return E_FAIL;
}

CMesh_Static * CMesh_Static::Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pFilePath, const _tchar* pFileName, const _matrix* pLocalMatrix)
{
	CMesh_Static* pInstance = new CMesh_Static(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype(pFilePath, pFileName, pLocalMatrix)))
	{
		MSG_BOX("Failed while Creating CMesh_Static");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMesh_Static::Clone_Component(void * pArg)
{
	CMesh_Static* pInstance = new CMesh_Static(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed while Creating CMesh_Static");
		Safe_Release(pInstance);
	}

	return pInstance; 
}

void CMesh_Static::Free()
{
	for (_ulong i = 0; i < m_dwNumMaterials; ++i)
	{
		for (size_t j = 0; j < MESHTEXTURE::TYPE_END; ++j)
		{
			Safe_Release(m_pMeshTexture[i].pTexture[j]);
		}
	}

	if (false == is_Clone())
	{
		Safe_Delete_Array(m_pMaterials);
		Safe_Delete_Array(m_pMeshTexture);
	}

	Safe_Release(m_pAdjacencyBuffer);
	Safe_Release(m_pMaterialsBuffer);
	Safe_Release(m_pMesh);

	CComponent::Free();
}
