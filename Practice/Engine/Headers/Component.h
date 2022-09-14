#pragma once

#include "Base.h"

BEGIN(Engine)

// Ŭ������ ���� �� �ִ� �Լ����� Ŭ����ȭ �Ѱ�, �پ��� �ٸ����̼��� ������Ʈ�� ���� ������

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;
public:
	LPDIRECT3DDEVICE9 Get_Graphic_Device() const {
		return m_pDevice; }
	_bool is_Clone() {
		return m_isClone; }
public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);
private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	_bool					m_isClone = false;
public:
	virtual CComponent* Clone_Component(void* pArg) = 0;
	virtual void Free();
};

END