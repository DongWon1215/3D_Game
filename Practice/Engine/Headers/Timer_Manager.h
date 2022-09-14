#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer;
class CTimer_Manager : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)
private:
	explicit CTimer_Manager();
	virtual ~CTimer_Manager() = default;
public:
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_double Compute_TimeDelta(const _tchar* pTimerTag);
private:
	unordered_map<const _tchar*, CTimer*>			m_Timers;
	typedef unordered_map<const _tchar*, CTimer*>	TIMERS;
private:
	CTimer* Find_Timer(const _tchar* pTimerTag);
public:
	virtual void Free();
};

END