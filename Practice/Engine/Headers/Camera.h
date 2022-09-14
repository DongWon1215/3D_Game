#pragma once

#include "GameObject.h"
#include "Transform.h"
#include "PipeLine.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		// vEye : 월드스페이스 상의 카메라위치
		// vAt : 월드스페이스 상의 카메라가 바라보는 점. 
		// vAxisY : 내 월드 좌표계에서의 y축 벡터.
		_vec3	vEye, vAt, vAxisY;

		_float	fFovy, fAspect, fNear, fFar;

		_float	fSpeedPerSec, fRotationPerSec;

	}CAMERADESC;
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;
public:
	// 카메라 객체의 프로토 타입을 생성한다
	virtual HRESULT Ready_GameObject_Prototype();

	// 복제된 카메라 객체를 준비한다
	virtual HRESULT Ready_GameObject(void* pArg);
	// 카메라 객체를 업데이트 시킨다.
	virtual _int Update_GameObject(_double TimeDelta);
	// 카메라 객체를 한박자 느리게 업데이트 시킨다.
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	// 카메라 객체를 랜더한다.
	virtual HRESULT Render_GameObject();
protected:
	CTransform*			m_pTransformCom = nullptr;
	CPipeLine*			m_pPipeLine = nullptr;
protected:
	_matrix				m_ViewMatrix;
	_matrix				m_ProjMatrix;
public:
	virtual CGameObject* Clone_GameObject(void* pArg) = 0;
	virtual void Free();
};

END