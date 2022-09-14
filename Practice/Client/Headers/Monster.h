#pragma once

#include "Client_Extend.h"
#include "GameObject.h"

#include "Collider.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CNavigation;
class CMesh_Dynamic;
END

class CPlayer;
class CMonster final : public CGameObject
{
public:
	typedef enum TagHitBox
	{
		HITBOX_HEAD, HITBOX_BODY, HITBOX_LEFTARM, HITBOX_LEFTCLAFT, HITBOX_RIGHTARM, HITBOX_RIGHTCLAFT, HITBOX_END
	}HITBOX;

private:
	explicit CMonster(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT Set_Collider_Show();

public:
	void Cheat();

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CCollider* m_pColliderCom[HITBOX_END] = { nullptr };
	CMesh_Dynamic* m_pMeshCom = nullptr;
	CPlayer* m_pPlayer = nullptr;		//���������� �����ص� �÷��̾� ��ü�� �޾ƿ��� �İ�ü
	CNavigation* m_pNavigationCom = nullptr;

private:
	_double								m_TimeDelta;				 //��ŸŸ��

	CCollider::COLLIDER_DESC			m_RLegCollider_Desc;		 //������ �ٸ� �ݶ��̴�
	CCollider::COLLIDER_DESC			m_LLegCollider_Desc;		 //���� �ٸ� �ݶ��̴�
	CCollider::COLLIDER_DESC			m_RArmCollider_Desc;		 //������ �� �ݶ��̴�
	CCollider::COLLIDER_DESC			m_LArmCollider_Desc;		 //���� �� �ݶ��̴�
	CCollider::COLLIDER_DESC			m_HeadCollider_Desc;		 //�Ӹ� �ݶ��̴�
	CCollider::COLLIDER_DESC			m_BodyCollider_Desc;		 //���� �ݶ��̴�

private:
	_matrix* m_matLeftArm, * m_matLeftCalf, 						 //���� ���� ��Ʈ����
		* m_matRightArm, * m_matRightCalf, 							 //���� ���� ��Ʈ����
		* m_matHead, * m_matBody;									 //���� ���� ��Ʈ����
	_matrix					m_matRealMtx[6];						 //������ ���� ��Ʈ����

private:
	_int m_iMaxHP = 100, m_iAtt = 3;								 //������ ü�� �� ���ݷ�
	_int m_iCurHp = 100;											 //���� ü��

	_float m_fCoolTime = 0.f;										 //���� ���� ��Ÿ��

	//_bool m_bisFinish = false;										 //�ִϸ��̼� ���� �� �Ǻ� ����
	_bool m_bIsHit = false;												 //�ǰ� ���� ���� ����
	_bool m_bIsShow = false;											 //�ݶ��̴� ���� �Ǻ� ����

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_OnTerrain();

private:
	_bool IsCollision();											  //���� ���� �� �ǰ� �Լ�
	_bool IsCanAttack(_double TimeDelta);							  //���� ���� ���� ���� �Ǵ� �Լ�

private:
	_bool SetAnimation(CCollider* pCollider);						  //�ִϸ��̼� ���� �Լ�
	void SetCollider();												  //�ݶ��̴� ����
	void ChasePlayer(_double TimeDelta);							  //�νĹ��� �� �÷��̾� ���ٽ� �÷��̾�� ����
	void UpdateCollider();											  //���� ���� �ݶ��̴� ����� ������Ʈ

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pDevice); // ������ �����Ѵ�.
	virtual CGameObject* Clone_GameObject(void* pArg); // �������� �����Ѵ�.
	virtual void Free();
};