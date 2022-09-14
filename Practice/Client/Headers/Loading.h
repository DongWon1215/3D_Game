#pragma once
#include "Client_Extend.h"
#include "Base.h"

BEGIN(Engine)
class CComponent_Manager;
END

BEGIN(Client)

class CLoading final : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLoading() = default;
public:
	SCENEID Get_SceneID() const {
		return m_eSceneID;
	}
	_uint Get_Complete() const {
		return m_iComplete;
	}
public:
	HRESULT Ready_Loading(SCENEID eSceneID);
public:
	static _uint APIENTRY Thread_Main(void* pArg);
private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	SCENEID					m_eSceneID;
	HANDLE					m_hThread;
	CRITICAL_SECTION		m_CS;
	_uint					m_iComplete;

private:
	HRESULT Ready_Stage();

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pDevice, SCENEID eSceneID);
	virtual void Free();
};
END