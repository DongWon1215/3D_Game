#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CCell;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
class CNavigation;
END

class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrain(const CTerrain& rhs);
	explicit CTerrain(const CTerrain & rhs, _uint XVertex, _uint ZVertex, _float fInterval);
	virtual ~CTerrain() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();
public:
	void SetPassIndex(_uint iIndex) {
		m_iPassIndex = iIndex;
	}
	void SetIsUp(bool IsTrue) {
		m_bIsUp = IsTrue;
	}
	void SetIsTerrain(bool IsTrue) {
		m_bIsTerrain = IsTrue;
	}

	void SetRadius(float fRadius) {
		m_fRange = fRadius;
	}

	bool CalculateDistance(_vec3  VertexPosition,_vec3 TargetVec);

	void CalculVector();
	void EraseCell();

public:
	void SetCells();

public:
	vector<_vec3> GetVertexVector();
	vector<CCell*> GetCellVector();
	_vec3 GetVertexPos();
	_uint GetCellNum() const { return m_iCellCount; }
	
private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pFilterCom = nullptr;
	CTexture*				m_pBrushCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Terrain*		m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

private:
	HRESULT Add_Component();
	bool	m_bIsKeyDown();
#ifdef  _DEBUG
	HRESULT RanderCell();
#endif //  _DEBUG

private:
	_uint			m_iPassIndex = 0;
	_uint			m_iCellCount = 0;
	_float			m_fRange = 0.f;
	bool			m_bIsUp = false;
	bool			m_bIsTerrain = true;

public:
	vector<_vec3> m_vVertex;
	typedef vector<_vec3> VERTEX;

	vector<_vec3> m_vRealVertex;
	typedef vector<_vec3> REALVERTEX;

	vector<CCell*>			m_Cells;
	typedef vector<CCell*>	CELLS;

	//키 상태 확인용
private:
	DWORD m_dwKey = 0;
	DWORD m_dwKeyDown = 0;

	const DWORD KEY_LBUTTON = 0x00000040;

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice); // 원본을 생성한다.
	virtual CGameObject* Clone_GameObject(void* pArg); // 복제본을 생성한다.
	virtual void Free();
};