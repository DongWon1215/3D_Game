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
	CPlayer*							m_pPlayer = nullptr;		//스테이지에 생성해둔 플레이어 객체를 받아오는 컴객체
	CShader*							m_pShaderCom = nullptr;
	CRenderer*							m_pRendererCom = nullptr;
	CCollider*							m_pColliderCom = nullptr;
	CCollider*							m_pSwordCollider = nullptr;
	CTransform*							m_pTransformCom = nullptr;
	CNavigation*						m_pNavigationCom = nullptr;
	CMesh_Dynamic*						m_pMeshCom = nullptr;

private:
	_double								m_TimeDelta;				 //델타타임

private:
	_int m_iMaxHP = 100, m_iAtt = 0;								 //몬스터의 체력 및 공격력
	_int m_iCurHp = 100;

	_float m_fCoolTime = 0.f;										 //몬스터 공격 쿨타임

	_bool m_bisFinish = false;										 //애니메이션 셋의 끝 판별 변수
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
	_bool IsCollision(_vec3 vRay, _vec3 vPosition);					  //몬스터 공격 및 피격 함수
	_bool IsCanAttack(_double TimeDelta);							  //몬스터 공격 가능 여부 판단 함수
	_bool SetAnimation(CCollider* pCollider);						  //애니메이션 설정 함수

private:
	void SetCollider();												  //콜라이더 정의
	void ChasePlayer(_double TimeDelta);							  //인식범위 내 플레이어 접근시 플레이어에게 접근
	void ConnectAnimation();										  //애니메이션 연결 함수

public:
	static CSkeleton* Create(LPDIRECT3DDEVICE9 pDevice); // 원본을 생성ㄴ하낟.
	virtual CGameObject* Clone_GameObject(void* pArg); // 복제본을 생성ㅇ한다.
	virtual void Free();
};