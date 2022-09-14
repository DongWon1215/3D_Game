#pragma once


namespace Engine
{

	typedef struct tagMeshTexture
	{
		enum TYPE { TYPE_DIFFUSE, TYPE_NORMAL, TYPE_SPECULAR, TYPE_EMISSIVE, TYPE_END };

		LPDIRECT3DTEXTURE9		pTexture[TYPE_END];
	}MESHTEXTURE;

	typedef struct tagD3DXMeshContainer_Derived : public D3DXMESHCONTAINER
	{
		MESHTEXTURE*		pMeshTexture;
		LPD3DXMESH			pOriginalMesh;
		_ulong				dwNumFrames;
		D3DXMATRIX*			pOffsetMatrices;
		D3DXMATRIX**		ppCombinedTransformationMatrices;
		D3DXMATRIX*			pRenderingMatrices;
	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagD3DXFrame_Derived : public D3DXFRAME
	{
		D3DXMATRIX			CombinedTransformationMatrix;
	}D3DXFRAME_DERIVED;


	typedef struct tagVertex_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXTEX;

	typedef struct tagVertex_Viewport_Texture
	{
		D3DXVECTOR4		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXVIEWPORTTEX;

	typedef struct tagVertex_Cube_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexUV;
	}VTXCUBETEX;

	typedef struct tagVertex_Normal_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexUV;
	}VTXNORTEX;

	typedef struct tagVertex_Normal_Cube_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR3		vTexUV;
	}VTXNORCUBETEX;


	typedef struct tagIndex16
	{
		unsigned short	_1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		unsigned long	_1, _2, _3;
	}INDEX32;
}


