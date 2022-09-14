#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END};
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END};
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END};

private:
	explicit CCell(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCell() = default;

public:
	_uint Get_Index() const { return m_iIndex;}

	_vec3 Get_Point(POINT eType) { return m_vShareVector[eType];}
	
	void SetNeighbor(NEIGHBOR eType, CCell* pCell) { m_pNeighbor[eType] = pCell;}

public:
	HRESULT Ready_Cell(_vec3 vPointA, _vec3 vPointB, _vec3 vPointC, _uint iIndex);
	_bool Compare_Point(_vec3 vSourPoint, _vec3 vDestPoint);
	_bool Check_Move(const _vec3* pResultPos, CCell** ppNeighborCell);
	_bool Is_In(const _vec3* pResultPos, LINE* pLineId);

#ifdef _DEBUG
public:
	HRESULT Render_Cell();
#endif // _DEBUG
#ifdef _DEBUG
private:
	LPD3DXLINE			m_pLine = nullptr;
#endif

private:
	_uint	m_iIndex;
	CCell* m_pNeighbor[NEIGHBOR_END];

	LPDIRECT3DDEVICE9 m_pDevice;

private:
	_vec3	m_vShareVector[POINT_END];
	_vec3	m_vShareLine[LINE_END];
	_vec3	m_vNormal[LINE_END];


public:
	static CCell* Create(LPDIRECT3DDEVICE9 pDevice, _vec3 vecA, _vec3 vecB, _vec3 vecC, _uint iIndex);
	virtual void Free();

};
END
