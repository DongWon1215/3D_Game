#pragma once

// CQuadTree : 가장 큰 쿼드트리를 의미한다.
#include "Base.h"

BEGIN(Engine)

class CFrustum;
class CPipeLine;
class CQuadTree final : public CBase
{
public:
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_RB, CHILD_LB, CHILD_END };
	enum NEIGHBOR { NEIGHBOR_LEFT, NEIGHBOR_RIGHT, NEIGHBOR_TOP, NEIGHBOR_BOTTOM, NEIGHBOR_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_RB, CORNER_LB, CORNER_END };
private:
	explicit CQuadTree(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CQuadTree() = default;
public:
	HRESULT Ready_QuadTree(_vec3* pPosition, _uint iNumVerticesX, _uint iNumVerticesZ);
	HRESULT Ready_QuadTree(_vec3* pPosition, _uint iLT, _uint iRT, _uint iRB, _uint iLB);
	HRESULT Culling_ToQuadTree(CFrustum* pFrustum, _vec3* pPosition, INDEX32* pIndices, _uint* pNumPolygons);
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
private:
	CQuadTree*			m_pChild[CHILD_END] = { nullptr };
	CQuadTree*			m_pNeighbor[NEIGHBOR_END] = { nullptr };
	_uint				m_iCornerIndex[CORNER_END] = { 0 };
	_float				m_fRadius = 0.f;
	_uint				m_iCenterIndex = 0;
	CPipeLine*			m_pPipeLine = nullptr;
private:
	HRESULT Make_Child(_vec3 * pPosition);
	HRESULT Make_Neighbor();
	_bool LevelofDetail(_vec3* pPosition);
public:
	static CQuadTree* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3* pPosition, _uint iNumVerticesX, _uint iNumVerticesZ);
	static CQuadTree* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3* pPosition, _uint iLT, _uint iRT, _uint iRB, _uint iLB);
	virtual void Free();
};

END