#pragma once

// 객체의 움직임을 관리한다.(객체의 상태를 표현한다. == m_WorldMatrix(m_vPosition, m_vScale, m_vRotation))

#include "Component.h"
#include "Navigation.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct tagStateDesc
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
	}STATEDESC;
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public: // Getter
	_vec3 Get_State(STATE eState) const {
		return *(_vec3*)&m_WorldMatrix.m[eState][0];
	}
	_float Get_Scale(STATE eState) const {
		return D3DXVec3Length((_vec3*)&m_WorldMatrix.m[eState][0]);
	}
	_matrix Get_WorldMatrixInverse() const;
	_matrix Get_WorldMatrix() const {
		return m_WorldMatrix;
	}
	_matrix* Get_WorldMatrixPointer() {
		return &m_WorldMatrix;
	}
public: // Setter
	void Set_State(STATE eState, _vec3 vData) {
		memcpy(&m_WorldMatrix.m[eState][0], &vData, sizeof(_vec3));
	}
	void Set_StateDesc(STATEDESC StateDesc);
	void Set_Rotation(const _vec3* pAxis, _float fRadian);
	void Set_Scale(_vec3	vScale); // 내가 지정한 사이즈로 변형.
public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);
public:
	HRESULT Go_Straight(_double TimeDelta);
	HRESULT Go_Backward(_double TimeDelta);
	HRESULT Go_Left(_double TimeDelta);
	HRESULT Go_Right(_double TimeDelta);
	HRESULT Rotation_Axis(const _vec3* pAxis, _double TimeDelta);
	_float Go_Target(_vec3 vTargetPos, _double TimeDelta);
	// HRESULT Scaling(_vec3 vScale); // 기존사이즈에서 더 증가, 감소.
	HRESULT Remove_Scale();

public:
	HRESULT Go_Straight(CNavigation* pNavigation, _double TimeDelta);
	HRESULT Go_Backward(CNavigation* pNavigation, _double TimeDelta);
	HRESULT Go_Left(CNavigation* pNavigation, _double TimeDelta);
	HRESULT Go_Right(CNavigation* pNavigation, _double TimeDelta);
	_float Go_Target(CNavigation* pNavigation, _vec3 vTargetPos, _double TimeDelta);
private:
	_matrix			m_WorldMatrix;
	STATEDESC		m_StateDesc;
public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END