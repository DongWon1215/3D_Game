#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMesh_Static final : public CComponent
{
private:
	explicit CMesh_Static(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh_Static(const CMesh_Static& rhs);
	virtual ~CMesh_Static() = default;
public:
	_ulong Get_NumSubset() const {
		return m_dwNumMaterials; }
	LPDIRECT3DTEXTURE9 Get_MaterialTexture(_ulong dwSubSetIndex, MESHTEXTURE::TYPE eType) const;
	D3DMATERIAL9 Get_Material(_ulong dwSubSetIndex) const {
		return m_pMaterials[dwSubSetIndex].MatD3D; }
public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pFilePath, const _tchar* pFileName, const _matrix* pLocalMatrix);
	virtual HRESULT Ready_Component(void* pArg);
public:
	HRESULT Render_Mesh(_uint iSubsetIndex);
private:
	LPD3DXMESH				m_pMesh = nullptr;

	LPD3DXBUFFER			m_pAdjacencyBuffer = nullptr;	// 인접 행렬
	LPD3DXBUFFER			m_pMaterialsBuffer = nullptr;	// 재질 행렬
	_ulong					m_dwNumMaterials = 0;			// 재질 개수
private:
	D3DXMATERIAL*			m_pMaterials = nullptr;
	MESHTEXTURE*			m_pMeshTexture = nullptr;

private:
	HRESULT Change_TextureFileName(_tchar* pOut, _tchar* pIn, const _tchar* pSourText, const _tchar* pDestText);


public:
	static CMesh_Static* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName, const _matrix* pLocalMatrix);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END