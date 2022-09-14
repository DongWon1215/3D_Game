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
		// vEye : ���彺���̽� ���� ī�޶���ġ
		// vAt : ���彺���̽� ���� ī�޶� �ٶ󺸴� ��. 
		// vAxisY : �� ���� ��ǥ�迡���� y�� ����.
		_vec3	vEye, vAt, vAxisY;

		_float	fFovy, fAspect, fNear, fFar;

		_float	fSpeedPerSec, fRotationPerSec;

	}CAMERADESC;
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;
public:
	// ī�޶� ��ü�� ������ Ÿ���� �����Ѵ�
	virtual HRESULT Ready_GameObject_Prototype();

	// ������ ī�޶� ��ü�� �غ��Ѵ�
	virtual HRESULT Ready_GameObject(void* pArg);
	// ī�޶� ��ü�� ������Ʈ ��Ų��.
	virtual _int Update_GameObject(_double TimeDelta);
	// ī�޶� ��ü�� �ѹ��� ������ ������Ʈ ��Ų��.
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	// ī�޶� ��ü�� �����Ѵ�.
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