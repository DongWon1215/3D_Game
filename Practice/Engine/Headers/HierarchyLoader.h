#pragma once

#include "Base.h"

BEGIN(Engine)

class CHierarchyLoader final : public ID3DXAllocateHierarchy, public CBase
{
private:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHierarchyLoader() = default;
public:
	HRESULT Ready_HierarchyLoader(const _tchar* pFilePath);
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame) ;
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	const _tchar*			m_pFilePath = nullptr;
private:
	HRESULT Allocate_Name(char** ppSourName, const char* pDestName);
	HRESULT Change_TextureFileName(_tchar* pOut, _tchar* pIn, const _tchar* pSourText, const _tchar* pDestText);
public:
	static CHierarchyLoader* Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pFilePath);
	virtual void Free();
};

END