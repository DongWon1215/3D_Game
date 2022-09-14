#include "..\Headers\PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
}

_matrix CPipeLine::Get_Transform(D3DTRANSFORMSTATETYPE eTransformType)
{
	_matrix		matTmp;

	D3DXMatrixIdentity(&matTmp);

	switch (eTransformType)
	{
	case D3DTS_VIEW:
		return m_ViewMatrix;
		break;
	case D3DTS_PROJECTION:
		return m_ProjMatrix;
		break;
	}

	return matTmp;
}

HRESULT CPipeLine::Set_Transform(D3DTRANSFORMSTATETYPE eTransformType, _matrix TransformMatrix)
{
	switch (eTransformType)
	{
	case D3DTS_VIEW:
		m_ViewMatrix = TransformMatrix;
		break;
	case D3DTS_PROJECTION:
		m_ProjMatrix = TransformMatrix;
		break;
	}
	return NOERROR;
}

void CPipeLine::Free()
{
}
