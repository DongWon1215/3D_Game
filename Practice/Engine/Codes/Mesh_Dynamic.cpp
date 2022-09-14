#include "..\Headers\Mesh_Dynamic.h"
#include "..\Headers\HierarchyLoader.h"
#include "..\Headers\AnimationControl.h"

CMesh_Dynamic::CMesh_Dynamic(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{

}

CMesh_Dynamic::CMesh_Dynamic(const CMesh_Dynamic & rhs)
	: CComponent(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_MeshContainers(rhs.m_MeshContainers)
	, m_pLoader(rhs.m_pLoader)
	, m_LocalMatrix(rhs.m_LocalMatrix)
	, m_pAnimationCtrl(rhs.m_pAnimationCtrl->Clone_AnimationCtrl())
{
	Safe_AddRef(m_pAnimationCtrl);
	Safe_AddRef(m_pLoader);
}

LPDIRECT3DTEXTURE9 CMesh_Dynamic::Get_MaterialTexture(_uint iMeshContainerIndex, _ulong dwSubSetIndex, MESHTEXTURE::TYPE eType) const
{
	if (m_MeshContainers.size() <= iMeshContainerIndex ||
		m_MeshContainers[iMeshContainerIndex]->NumMaterials <= dwSubSetIndex ||
		nullptr == m_MeshContainers[iMeshContainerIndex]->pMeshTexture)
		return nullptr;

	return m_MeshContainers[iMeshContainerIndex]->pMeshTexture[dwSubSetIndex].pTexture[eType];
}

HRESULT CMesh_Dynamic::Ready_Component_Prototype(const _tchar * pFilePath, const _tchar * pFileName, const _matrix * pLocalMatrix)
{
	_tchar		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(Get_Graphic_Device(), pFilePath);
	if (nullptr == m_pLoader)
		return E_FAIL;

	LPD3DXANIMATIONCONTROLLER		pAnimationCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, Get_Graphic_Device(), m_pLoader, nullptr, &m_pRootFrame, &pAnimationCtrl)))
		return E_FAIL;

	m_LocalMatrix = *pLocalMatrix;

	m_pAnimationCtrl = CAnimationControl::Create(pAnimationCtrl);
	if (nullptr == m_pAnimationCtrl)
		return E_FAIL;

	if (FAILED(SetUp_CombinedTransformationMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame)))
		return E_FAIL; 


	return NOERROR;
}

HRESULT CMesh_Dynamic::Ready_Component(void * pArg)
{
	m_pCopyFrame = Copy_Frames((D3DXFRAME_DERIVED*)m_pRootFrame);

	LPD3DXFRAME	pFrame = D3DXFrameFind(m_pRootFrame, "R_Hand");
	pFrame = D3DXFrameFind(m_pCopyFrame, "R_Hand");

	if (FAILED(Update_CombinedTransformationMatrix((D3DXFRAME_DERIVED*)m_pRootFrame, (D3DXFRAME_DERIVED*)m_pCopyFrame, m_LocalMatrix)))
		return E_FAIL;

	if (FAILED(SetUp_CombinedTransformationMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMesh_Dynamic::Update_SkinnedMesh(_uint iMeshContainerIndex)
{
	if (m_MeshContainers.size() <= iMeshContainerIndex)
		return E_FAIL;

	for (_ulong i = 0; i < m_MeshContainers[iMeshContainerIndex]->dwNumFrames; ++i)
		m_MeshContainers[iMeshContainerIndex]->pRenderingMatrices[i] = m_MeshContainers[iMeshContainerIndex]->pOffsetMatrices[i] * *m_MeshContainers[iMeshContainerIndex]->ppCombinedTransformationMatrices[i];

	void*	pVerticesSour, *pVerticesDest;
	
	m_MeshContainers[iMeshContainerIndex]->pOriginalMesh->LockVertexBuffer(0, &pVerticesSour);
	m_MeshContainers[iMeshContainerIndex]->MeshData.pMesh->LockVertexBuffer(0, &pVerticesDest);
	
	m_MeshContainers[iMeshContainerIndex]->pSkinInfo->UpdateSkinnedMesh(m_MeshContainers[iMeshContainerIndex]->pRenderingMatrices, nullptr, pVerticesSour, pVerticesDest);
	m_MeshContainers[iMeshContainerIndex]->pOriginalMesh->UnlockVertexBuffer();
	m_MeshContainers[iMeshContainerIndex]->MeshData.pMesh->UnlockVertexBuffer();

	return NOERROR;
}

HRESULT CMesh_Dynamic::Render_Mesh(_uint iMeshContainerIndex, _ulong dwSubSetIndex)
{
	if (m_MeshContainers.size() <= iMeshContainerIndex ||
		m_MeshContainers[iMeshContainerIndex]->NumMaterials <= dwSubSetIndex)
		return E_FAIL;

	m_MeshContainers[iMeshContainerIndex]->MeshData.pMesh->DrawSubset(dwSubSetIndex);

	return NOERROR;
}

HRESULT CMesh_Dynamic::SetUp_AnimationSet(_uint iIndex)
{
	if (nullptr == m_pAnimationCtrl)
		return E_FAIL;

	return m_pAnimationCtrl->SetUp_AnimationSet(iIndex);
}

HRESULT CMesh_Dynamic::Play_AnimationSet(_double TimeDelta)
{
	if (nullptr == m_pAnimationCtrl)
		return E_FAIL;

	m_pAnimationCtrl->Play_AnimationSet(TimeDelta);

	if (FAILED(Update_CombinedTransformationMatrix((D3DXFRAME_DERIVED*)m_pRootFrame, (D3DXFRAME_DERIVED*)m_pCopyFrame, m_LocalMatrix)))
		return E_FAIL;

	return NOERROR;
}

_bool CMesh_Dynamic::is_Finished(_double pTime) const
{
	if (nullptr == m_pAnimationCtrl)
		return false;

	return m_pAnimationCtrl->is_Finished(pTime);
}

_uint CMesh_Dynamic::Get_CurrentTrack()
{
	return m_pAnimationCtrl->Get_CurrentTrack();
}

HRESULT CMesh_Dynamic::Update_CombinedTransformationMatrix(D3DXFRAME_DERIVED* pFrame, D3DXFRAME_DERIVED* pCopyFrame, _matrix ParentMatrix)
{
	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * ParentMatrix;


	if (nullptr != pFrame->pFrameFirstChild)
		Update_CombinedTransformationMatrix((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, (D3DXFRAME_DERIVED*)pCopyFrame->pFrameFirstChild, pFrame->CombinedTransformationMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_CombinedTransformationMatrix((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, (D3DXFRAME_DERIVED*)pCopyFrame->pFrameSibling, ParentMatrix);

	return NOERROR;
}

D3DXFRAME_DERIVED* CMesh_Dynamic::Copy_Frames(D3DXFRAME_DERIVED * pFrame)
{
	D3DXFRAME_DERIVED*	pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));

	if (nullptr != pFrame->Name)
	{
		size_t iLength = strlen(pFrame->Name);

		pNewFrame->Name = new char[iLength + 1];
		ZeroMemory(pNewFrame->Name, sizeof(char) * iLength + 1);

		strcpy(pNewFrame->Name, pFrame->Name);
	}

	if (nullptr != pFrame->pFrameFirstChild)
		pNewFrame->pFrameFirstChild = Copy_Frames((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);

	if (nullptr != pFrame->pFrameSibling)
		pNewFrame->pFrameSibling = Copy_Frames((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	return pNewFrame;
}

// 메시컨테이너안에 있는 ppCombinedTransformationMatrices배열에 뼈의 Combined행렬의 주소를 담는다.
HRESULT CMesh_Dynamic::SetUp_CombinedTransformationMatrixPointer(D3DXFRAME_DERIVED * pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*	pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pMeshContainer->dwNumFrames; ++i)
		{
			D3DXFRAME_DERIVED*	pFindFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pMeshContainer->pSkinInfo->GetBoneName(i));

			pMeshContainer->ppCombinedTransformationMatrices[i] = &pFindFrame->CombinedTransformationMatrix;
		}

		m_MeshContainers.push_back(pMeshContainer);

	}

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_CombinedTransformationMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);

	if (nullptr != pFrame->pFrameSibling)
		SetUp_CombinedTransformationMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);


	return NOERROR;
}

CMesh_Dynamic * CMesh_Dynamic::Create(LPDIRECT3DDEVICE9 pDevice, const _tchar * pFilePath, const _tchar * pFileName, const _matrix* pLocalMatrix)
{
	CMesh_Dynamic* pInstance = new CMesh_Dynamic(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype(pFilePath, pFileName, pLocalMatrix)))
	{
		MSG_BOX("Failed while Creating CMesh_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMesh_Dynamic::Clone_Component(void * pArg)
{
	CMesh_Dynamic* pInstance = new CMesh_Dynamic(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed while Creating CMesh_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh_Dynamic::Free()
{
	if (false == is_Clone())
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
	}

	Safe_Release(m_pAnimationCtrl);
	Safe_Release(m_pLoader);

	CComponent::Free();
}
