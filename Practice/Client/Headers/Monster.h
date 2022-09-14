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
	CPlayer* m_pPlayer = nullptr;		//스테이지에 생성해둔 플레이어 객체를 받아오는 컴객체
	CNavigation* m_pNavigationCom = nullptr;

private:
	_double								m_TimeDelta;				 //델타타임

	CCollider::COLLIDER_DESC			m_RLegCollider_Desc;		 //오른쪽 다리 콜라이더
	CCollider::COLLIDER_DESC			m_LLegCollider_Desc;		 //왼쪽 다리 콜라이더
	CCollider::COLLIDER_DESC			m_RArmCollider_Desc;		 //오른쪽 팔 콜라이더
	CCollider::COLLIDER_DESC			m_LArmCollider_Desc;		 //왼쪽 팔 콜라이더
	CCollider::COLLIDER_DESC			m_HeadCollider_Desc;		 //머리 콜라이더
	CCollider::COLLIDER_DESC			m_BodyCollider_Desc;		 //몸통 콜라이더

private:
	_matrix* m_matLeftArm, * m_matLeftCalf, 						 //몬스터 뼈의 매트릭스
		* m_matRightArm, * m_matRightCalf, 							 //몬스터 뼈의 매트릭스
		* m_matHead, * m_matBody;									 //몬스터 뼈의 매트릭스
	_matrix					m_matRealMtx[6];						 //실제로 사용될 매트릭스

private:
	_int m_iMaxHP = 100, m_iAtt = 3;								 //몬스터의 체력 및 공격력
	_int m_iCurHp = 100;											 //현재 체력

	_float m_fCoolTime = 0.f;										 //몬스터 공격 쿨타임

	//_bool m_bisFinish = false;										 //애니메이션 셋의 끝 판별 변수
	_bool m_bIsHit = false;												 //피격 여부 판정 변수
	_bool m_bIsShow = false;											 //콜라이더 가시 판별 변수

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_OnTerrain();

private:
	_bool IsCollision();											  //몬스터 공격 및 피격 함수
	_bool IsCanAttack(_double TimeDelta);							  //몬스터 공격 가능 여부 판단 함수

private:
	_bool SetAnimation(CCollider* pCollider);						  //애니메이션 설정 함수
	void SetCollider();												  //콜라이더 정의
	void ChasePlayer(_double TimeDelta);							  //인식범위 내 플레이어 접근시 플레이어에게 접근
	void UpdateCollider();											  //뼈에 붙인 콜라이더 행렬의 업데이트

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pDevice); // 원본을 생성한다.
	virtual CGameObject* Clone_GameObject(void* pArg); // 복제본을 생성한다.
	virtual void Free();
};