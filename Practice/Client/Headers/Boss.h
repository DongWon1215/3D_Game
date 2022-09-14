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
class CBoss final : public CGameObject
{
public:
	typedef enum TagHitBox
	{
		HITBOX_HEAD, HITBOX_BODY, HITBOX_LEFTARM, HITBOX_LEFTCLAFT, HITBOX_RIGHTARM, HITBOX_RIGHTCLAFT, HITBOX_END
	}HITBOX;

private:
	explicit CBoss(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoss(const CBoss& rhs);
	virtual ~CBoss() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	_uint Get_Hp() const { return m_iCurHp; }

public:
	void Cheat();

private:
	CShader*							m_pShaderCom = nullptr;
	CRenderer*							m_pRendererCom = nullptr;
	CTransform*							m_pTransformCom = nullptr;
	CCollider*							m_pColliderCom[HITBOX_END] = { nullptr };
	CMesh_Dynamic*						m_pMeshCom = nullptr;
	CNavigation*						m_pNavigationCom = nullptr;
	CPlayer*							m_pPlayer = nullptr;		//���������� �����ص� �÷��̾� ��ü�� �޾ƿ��� �İ�ü

private:
	_double								m_TimeDelta;				 //��ŸŸ��

	CCollider::COLLIDER_DESC			m_RLegCollider_Desc;		 //������ �ٸ� �ݶ��̴�
	CCollider::COLLIDER_DESC			m_LLegCollider_Desc;		 //���� �ٸ� �ݶ��̴�
	CCollider::COLLIDER_DESC			m_RArmCollider_Desc;		 //������ �� �ݶ��̴�
	CCollider::COLLIDER_DESC			m_LArmCollider_Desc;		 //���� �� �ݶ��̴�
	CCollider::COLLIDER_DESC			m_HeadCollider_Desc;		 //�Ӹ� �ݶ��̴�
	CCollider::COLLIDER_DESC			m_BodyCollider_Desc;		 //���� �ݶ��̴�

private:
	_matrix*				m_matLeftArm, *m_matLeftCalf, 				//���� ���� ��Ʈ����
							*m_matRightArm, *m_matRightCalf, 			//���� ���� ��Ʈ����
							*m_matHead, *m_matBody;						//���� ���� ��Ʈ����
	_matrix					m_matRealMtx[6];							//������ ���� ��Ʈ����

private:
	_int m_iMaxHP = 1000, m_iAtt = 50;									//������ �ִ� ü�� �� ���ݷ�
	_int m_iCurHp = 1000;												//������ ���� ü��
	_uint m_iHitCount = 0;

	_float m_fCoolTime = 0.f;											//���� ���� ��Ÿ��

	_bool m_bIsHit = false;												//�ǰ� �Ǻ� ����
	_bool m_bIsFury = false;											//���� �г� �Ǻ� ����
	_bool m_bIsShow = false;
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_OnTerrain();

private:
	_bool SetAnimation(CCollider* pCollider);						  //�ִϸ��̼� ���� �Լ�

private:
	_bool IsCollision();											  //���� ���� �� �ǰ� �Լ�
	_bool IsCanAttack(_double TimeDelta);							  //���� ���� ���� ���� �Ǵ� �Լ�
	_bool IsAttack(_double TimeDelta);
	_bool Fury();													  //ü�� 50% ���Ͻ� �ִϸ��̼� ����

private:
	void SetCollider();												  //�ݶ��̴� ����
	void ChasePlayer(_double TimeDelta);							  //�νĹ��� �� �÷��̾� ���ٽ� �÷��̾�� ����
	void UpdateCollider();											  //���� ���� �ݶ��̴� ����� ������Ʈ
	void Bleeding();

private:
	HRESULT AttackPattern1();										  //���� ��� �� ���� ����
	HRESULT AttackPattern2(_double TimeDelta);						  //���� ��� �� ���� ����

public:
	static CBoss* Create(LPDIRECT3DDEVICE9 pDevice); // ������ �����Ѵ�.
	virtual CGameObject* Clone_GameObject(void* pArg); // �������� �����Ѵ�.
	virtual void Free();
};