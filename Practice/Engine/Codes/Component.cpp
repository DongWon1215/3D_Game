#include "..\Headers\Component.h"

CComponent::CComponent(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
	, m_isClone(false)
{
	Safe_AddRef(m_pDevice);
}

CComponent::CComponent(const CComponent & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_isClone(true)
{
	Safe_AddRef(m_pDevice);
}

HRESULT CComponent::Ready_Component_Prototype()
{
	return NOERROR;
}

HRESULT CComponent::Ready_Component(void * pArg)
{
	return NOERROR;
}

void CComponent::Free()
{
	Safe_Release(m_pDevice);
}
