#pragma once

// 1. ��� Ŭ������ �θ� �ȴ�.(�߻�Ŭ����)
// 2. ���۷���ī��Ʈ�� �������ϳ�.(���۷���ī��Ʈ�� ������Ų��. or ���ҽ�Ų�� or �����Ѵ�.)

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	explicit CBase();
	virtual ~CBase() = default;
public:
	_ulong AddRef(); // ������Ų��.
	_ulong Release(); // ���ҽ�Ų�� or �����Ѵ�.
private:
	_ulong	m_dwRefCnt = 0;
public:
	virtual void Free() = 0;
};

END