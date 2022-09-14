#pragma once

#include "Client_Extend.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CTransform;
class CNavigation;
class CMesh_Dynamic;
END

class CPlayer;
class CSkeleton final : public CGameObject
{
public:
	typedef enum TagState
	{
		STATE_IDLE, STATE_CHASE, STATE_ATTACK1, STATE_ATTACK2, STATE_ATTACK3, STATE_HIT, STATE_DOWN, STATE_DEAD, STATE_END
	}SKELETONSTATE;
private:
	explicit CSkeleton(LPDIRECT3DDEVICE9 pDevice);
	explicit CSkeleton(const CSkeleton& rhs);
	virtual ~CSkeleton() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CPlayer*							m_pPlayer = nullptr;		//���������� �����ص� �÷��̾� ��ü�� �޾ƿ��� �İ�ü
	CShader*							m_pShaderCom = nullptr;
	CRenderer*							m_pRendererCom = nullptr;
	CCollider*							m_pColliderCom = nullptr;
	CCollider*							m_pSwordCollider = nullptr;
	CTransform*							m_pTransformCom = nullptr;
	CNavigation*						m_pNavigationCom = nullptr;
	CMesh_Dynamic*						m_pMeshCom = nullptr;

private:
	_double								m_TimeDelta;				 //��ŸŸ��

private:
	_int m_iMaxHP = 100, m_iAtt = 0;								 //������ ü�� �� ���ݷ�
	_int m_iCurHp = 100;

	_float m_fCoolTime = 0.f;										 //���� ���� ��Ÿ��

	_bool m_bisFinish = false;										 //�ִϸ��̼� ���� �� �Ǻ� ����
	_bool m_bIsHit = false;

	_matrix* m_pMatWeapon;

	_matrix m_MatWeapon;

	SKELETONSTATE m_ePreState;
	SKELETONSTATE m_eCurState;
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_OnTerrain();

private:
	_bool IsCollision(_vec3 vRay, _vec3 vPosition);					  //���� ���� �� �ǰ� �Լ�
	_bool IsCanAttack(_double TimeDelta);							  //���� ���� ���� ���� �Ǵ� �Լ�
	_bool SetAnimation(CCollider* pCollider);						  //�ִϸ��̼� ���� �Լ�

private:
	void SetCollider();												  //�ݶ��̴� ����
	void ChasePlayer(_double TimeDelta);							  //�νĹ��� �� �÷��̾� ���ٽ� �÷��̾�� ����
	void ConnectAnimation();										  //�ִϸ��̼� ���� �Լ�

public:
	static CSkeleton* Create(LPDIRECT3DDEVICE9 pDevice); // ������ �������ϳ�.
	virtual CGameObject* Clone_GameObject(void* pArg); // �������� �������Ѵ�.
	virtual void Free();
};