#pragma once

// 게임내 객체들을 그릴때(렌더링파이프라인을 거칠때) 반드시 필요한 행렬변환 정보를 가지고 있는다.
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