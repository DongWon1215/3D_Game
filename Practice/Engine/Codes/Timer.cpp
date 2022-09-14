#include "..\Headers\Timer.h"

CTimer::CTimer()
{
}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);

	return NOERROR;
}

_double CTimer::Compute_TimerDelta()
{
	QueryPerformanceCounter(&m_FrameTime);

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	_double		TimeDelta = _double(m_FrameTime.QuadPart - m_LastTime.QuadPart) / m_CpuTick.QuadPart;

	QueryPerformanceCounter(&m_LastTime);

	return _double(TimeDelta);
}

CTimer * CTimer::Create()
{
	CTimer* pInstance = new CTimer();

	if (FAILED(pInstance->Ready_Timer()))
	{
		MSG_BOX("Failed while Creating CTimer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimer::Free()
{

}
