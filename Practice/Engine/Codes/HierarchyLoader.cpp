#include "..\Headers\HierarchyLoader.h"

CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	Safe_AddRef(m_pDevice);
}

HRESULT CHierarchyLoader::Ready_HierarchyLoader(const _tchar* pFilePath)
{
	m_pFilePath = pFilePath;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED*			pFrame_Derived = new D3DXFRAME_DERIVED;
	ZeroMemory(pFrame_Derived, sizeof(D3DXFRAME_DERIVED));

	if (nullptr != Name)
		Allocate_Name(&pFrame_Derived->Name, Name);

	pFrame_Derived->CombinedTransformationMatrix = *D3DXMatrixIdentity(&pFrame_Derived->TransformationMatrix);

	*ppNewFrame = pFrame_Derived;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*	pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	if (nullptr != Name)
		Allocate_Name(&pMeshContainer->Name, Name);

	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH		pMesh = pMeshData->pMesh;
	Safe_AddRef(pMesh);

	//if (FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF() /*| D3DFVF_NORMAL*/, m_pDevice, &pMeshContainer->MeshData.pMesh)))
	//	return E_FAIL;

	/*D3DVERTEXELEMENT9			Decl[MAX_FVF_DECL_SIZE] = {
	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	D3DDECL_END()
	};

	if (FAILED(pMesh->CloneMesh(pMesh->GetOptions(), Decl, m_pDevice, &pMeshContainer->MeshData.pMesh)))
	return E_FAIL;*/

	D3DVERTEXELEMENT9 TempElement[MAX_FVF_DECL_SIZE];
	pMesh->GetDeclaration(TempElement);
	WORD ByteAcc = 0;
	for (int i = 0; i < MAX_FVF_DECL_SIZE; i++)
	{
		ByteAcc = TempElement[i - 1].Offset + (TempElement[i - 1].Type + 1) * 4 ;
		if (TempElement[i].Stream == (BYTE)255)
		{
			TempElement[i] = { 0, ByteAcc, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 };
			TempElement[i + 1] = D3DDECL_END();
			break;
		}
	}

	if (FAILED(pMesh->CloneMesh(pMesh->GetOptions(), TempElement, m_pDevice, &pMeshContainer->MeshData.pMesh)))
		return E_FAIL;

	Safe_Release(pMesh);

	pMeshContainer->NumMaterials = NumMaterials;

	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pMeshContainer->NumMaterials);

	pMeshContainer->pMeshTexture = new MESHTEXTURE[pMeshContainer->NumMaterials];
	ZeroMemory(pMeshContainer->pMeshTexture, sizeof(MESHTEXTURE) * pMeshContainer->NumMaterials);


	_tchar		szFullPath[MAX_PATH] = L"";

	for (size_t i = 0; i < pMeshContainer->NumMaterials; ++i)
	{
		lstrcpy(szFullPath, m_pFilePath);

		_tchar		szFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_ACP, 0, pMeshContainer->pMaterials[i].pTextureFilename, strlen(pMeshContainer->pMaterials[i].pTextureFilename)
			, szFileName, MAX_PATH);

		lstrcat(szFullPath, szFileName);

		_tchar		szOut[MAX_PATH] = L"";

		// For.Diffuse_Map
		Change_TextureFileName(szOut, szFullPath, L"D", L"D");
		D3DXCreateTextureFromFile(m_pDevice, szOut, &pMeshContainer->pMeshTexture[i].pTexture[MESHTEXTURE::TYPE_DIFFUSE]);

		// For.Normal_Map	
		Change_TextureFileName(szOut, szFullPath, L"D", L"N");
		D3DXCreateTextureFromFile(m_pDevice, szOut, &pMeshContainer->pMeshTexture[i].pTexture[MESHTEXTURE::TYPE_NORMAL]);

		// For.Specular_Map
		Change_TextureFileName(szOut, szFullPath, L"D", L"S");
		D3DXCreateTextureFromFile(m_pDevice, szOut, &pMeshContainer->pMeshTexture[i].pTexture[MESHTEXTURE::TYPE_SPECULAR]);

		// For.Emissive_Map
		Change_TextureFileName(szOut, szFullPath, L"D", L"E");
		D3DXCreateTextureFromFile(m_pDevice, szOut, &pMeshContainer->pMeshTexture[i].pTexture[MESHTEXTURE::TYPE_EMISSIVE]);
	}

	pMeshContainer->pAdjacency = new _ulong[pMeshContainer->MeshData.pMesh->GetNumFaces() * 3];
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * pMeshContainer->MeshData.pMesh->GetNumFaces() * 3);

	pMeshContainer->pSkinInfo = pSkinInfo;
	Safe_AddRef(pMeshContainer->pSkinInfo);

	pMeshContainer->dwNumFrames = pMeshContainer->pSkinInfo->GetNumBones();

	pMeshContainer->pOffsetMatrices = new _matrix[pMeshContainer->dwNumFrames];

	for (_ulong i = 0; i < pMeshContainer->dwNumFrames; ++i)
	{
		pMeshContainer->pOffsetMatrices[i] = *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
	}

	pMeshContainer->pSkinInfo->SetDeclaration(TempElement);

	D3DXComputeTangent(pMesh, 0, 0, 0, 0, pAdjacency);

	pMeshContainer->ppCombinedTransformationMatrices = new _matrix*[pMeshContainer->dwNumFrames];
	pMeshContainer->pRenderingMatrices = new _matrix[pMeshContainer->dwNumFrames];

	if (FAILED(pMeshContainer->MeshData.pMesh->CloneMesh(pMeshContainer->MeshData.pMesh->GetOptions(), TempElement, m_pDevice, &pMeshContainer->pOriginalMesh)))
		return E_FAIL;

	*ppNewMeshContainer = pMeshContainer;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	Safe_Delete(pFrameToFree);

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*	pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->pAdjacency);
	Safe_Delete_Array(pMeshContainer->pOffsetMatrices);
	Safe_Delete_Array(pMeshContainer->ppCombinedTransformationMatrices);
	Safe_Delete_Array(pMeshContainer->pRenderingMatrices);
	Safe_Release(pMeshContainer->MeshData.pMesh);
	Safe_Release(pMeshContainer->pSkinInfo);
	Safe_Release(pMeshContainer->pOriginalMesh);

	for (size_t i = 0; i < pMeshContainer->NumMaterials; ++i)
	{
		for (size_t j = 0; j < MESHTEXTURE::TYPE_END; ++j)
			Safe_Release(pMeshContainer->pMeshTexture[i].pTexture[j]);
	}

	Safe_Delete_Array(pMeshContainer->pMeshTexture);

	Safe_Delete(pMeshContainer);

	return NOERROR;
}

HRESULT CHierarchyLoader::Allocate_Name(char ** ppSourName, const char * pDestName)
{
	size_t iLength = strlen(pDestName);

	*ppSourName = new char[iLength + 1];
	ZeroMemory(*ppSourName, sizeof(char) * iLength + 1);

	strcpy(*ppSourName, pDestName);

	return NOERROR;
}

HRESULT CHierarchyLoader::Change_TextureFileName(_tchar* pOut, _tchar* pIn, const _tchar* pSourText, const _tchar* pDestText)
{
	_uint iLength = lstrlen(pIn);

	lstrcpy(pOut, pIn);

	for (size_t i = iLength - 4; i >= 0; --i)
	{
		if (pIn[i] == *pSourText)
		{
			pOut[i] = *pDestText;
			break;
		}
	}

	return NOERROR;
}


CHierarchyLoader * CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pFilePath)
{
	CHierarchyLoader* pInstance = new CHierarchyLoader(pDevice);

	if (FAILED(pInstance->Ready_HierarchyLoader(pFilePath)))
	{
		MSG_BOX("Failed while Creating CHierarchyLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHierarchyLoader::Free()
{
	Safe_Release(m_pDevice);
}
