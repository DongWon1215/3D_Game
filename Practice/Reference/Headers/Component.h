#pragma once

#include "Base.h"

BEGIN(Engine)

// 클래스가 가질 수 있는 함수들을 클래스화 한것, 다양한 바리에이션의 오브젝트를 생성 가능함

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