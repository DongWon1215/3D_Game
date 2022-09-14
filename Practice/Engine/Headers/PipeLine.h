#pragma once

// ���ӳ� ��ü���� �׸���(������������������ ��ĥ��) �ݵ�� �ʿ��� ��ĺ�ȯ ������ ������ �ִ´�.
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)
private:
	explicit CPipeLine();
	virtual ~CPipeLine() = default;
public:
	_matrix Get_Transform(D3DTRANSFORMSTATETYPE eTransformType);
public:
	HRESULT Set_Transform(D3DTRANSFORMSTATETYPE eTransformType, _matrix TransformMatrix);
private:
	_matrix			m_ViewMatrix, m_ProjMatrix;	
public:	
	virtual void Free();
};

END