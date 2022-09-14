#include "..\Headers\Base.h"



CBase::CBase()
	: m_dwRefCnt(0)
{

}

_ulong CBase::AddRef()
{
	return ++m_dwRefCnt;
}

_ulong CBase::Release()
{
	if (0 == m_dwRefCnt) // 레퍼런스카운트가 영이면 지운다.
	{
		Free();		

		delete this;
		
		return 0;
	}

	else // 영이아니면 감소시킨다.
		return m_dwRefCnt--;		
}