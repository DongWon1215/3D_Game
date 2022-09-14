#pragma once

#include "Client_Extend.h"
#include "Base.h"
#include "Management.h"

BEGIN(Client)

class CMainGame final : public CBase
{
private:
	explicit CMainGame();
	virtual ~CMainGame() = default;

public:
	HRESULT Ready_MainApp();
	int Update_MainApp(double TimeDelta);
	HRESULT Render_MainApp();

private:
	CManagement*		m_pManagement = nullptr;

private:
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;
	CRenderer*			m_pRenderer = nullptr;

private:
	_uint				m_iRenderCnt = 0;
	_double				m_TimeAcc = 0.0;

private:
	HRESULT Ready_Default_Setting(CDevice::MODE eMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_Current_Scene(SCENEID eSceneID);
	HRESULT Ready_Component_Prototype();

public:
	static CMainGame* Create();
	virtual void Free(); // 나의 멤버변수를 정리한다.

};

END