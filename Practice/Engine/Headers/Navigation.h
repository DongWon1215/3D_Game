#pragma once
#include "Component.h"

BEGIN(Engine)

class CCell;
class ENGINE_DLL CNavigation final : public CComponent
{
private:
	explicit CNavigation(LPDIRECT3DDEVICE9 pDevice);
	explicit CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pFilePath);
	virtual HRESULT Ready_Component(void* pArg);

public:
	_bool is_Move(const _vec3* pResultPos);

public:
	void Setting_Index(_vec3* TargetPos);

#ifdef _DEBUG
public:
	HRESULT Render_Navigation();
#endif // _DEBUG

private:
	_uint			m_iNumCells = 0;
	_uint			m_iCurrentIndex = 0;

private:
	vector<CCell*>			m_Cells;
	typedef vector<CCell*>	CELLS;

private:
	HRESULT Set_Neighbor();

public:
	static CNavigation* Create(LPDIRECT3DDEVICE9 pDevice , const _tchar* pFilePath);
	virtual CComponent * Clone_Component(void * pArg);
	virtual void Free();
};

END