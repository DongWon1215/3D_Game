#pragma once

#include "Component.h"

BEGIN(Engine)

class CAnimationControl;
class CHierarchyLoader;
class ENGINE_DLL CMesh_Dynamic final : public CComponent
{
private:
	explicit CMesh_Dynamic(LPDIRECT3DDEVICE9 pDevice);
	explicit CMesh_Dynamic(const CMesh_Dynamic& rhs);
	virtual ~CMesh_Dynamic() = default;
public:

	D3DXFRAME_DERIVED* Get_FrameDerived(const char* pFrameName) {
		return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
	}
	_uint Get_NumMeshContainer() const {
		return m_MeshContainers.size();
	}
	_ulong Get_NumSubset(_uint iMeshContainerIndex) const {
		return m_MeshContainers[iMeshContainerIndex]->NumMaterials;
	}
	LPDIRECT3DTEXTURE9 Get_MaterialTexture(_uint iMeshContainerIndex, _ulong dwSubSetIndex, MESHTEXTURE::TYPE eType) const;

	D3DMATERIAL9 Get_Material(_uint iMeshContainerIndex, _ulong dwSubSetIndex) const {
		return m_MeshContainers[iMeshContainerIndex]->pMaterials[dwSubSetIndex].MatD3D;
	}
public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pFilePath, const _tchar* pFileName, const _matrix* pLocalMatrix);
	virtual HRESULT Ready_Component(void* pArg);
public:
	HRESULT Update_SkinnedMesh(_uint iMeshContainerIndex);
	HRESULT Render_Mesh(_uint iMeshContainerIndex, _ulong dwSubSetIndex);
	HRESULT SetUp_AnimationSet(_uint iIndex);
	HRESULT Play_AnimationSet(_double TimeDelta);
	_bool is_Finished(_double pTime = 0.0) const;

public:
	_uint Get_CurrentTrack();
private:
	D3DXFRAME*			m_pRootFrame = nullptr;
	D3DXFRAME*			m_pCopyFrame = nullptr;
	CHierarchyLoader*	m_pLoader = nullptr;
	CAnimationControl*		m_pAnimationCtrl = nullptr;
	_matrix				m_LocalMatrix;
private:
	vector<D3DXMESHCONTAINER_DERIVED*>		m_MeshContainers;
private:
	HRESULT Update_CombinedTransformationMatrix(D3DXFRAME_DERIVED* pFrame, D3DXFRAME_DERIVED* pCopyFrame, _matrix ParentMatrix);
	D3DXFRAME_DERIVED* Copy_Frames(D3DXFRAME_DERIVED* pFrame);
	HRESULT SetUp_CombinedTransformationMatrixPointer(D3DXFRAME_DERIVED* pFrame);
public:
	static CMesh_Dynamic* Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pFilePath, const _tchar* pFileName, const _matrix* pLocalMatrix);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END