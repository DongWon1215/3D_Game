#pragma once

// 1. 모든 클래스의 부모가 된다.(추상클래스)
// 2. 레퍼런스카운트르 ㄹ관리하낟.(레퍼런스카운트를 증가시킨다. or 감소시킨다 or 삭제한다.)

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	explicit CBase();
	virtual ~CBase() = default;
public:
	_ulong AddRef(); // 증가시킨다.
	_ulong Release(); // 감소시킨다 or 삭제한다.
private:
	_ulong	m_dwRefCnt = 0;
public:
	virtual void Free() = 0;
};

END