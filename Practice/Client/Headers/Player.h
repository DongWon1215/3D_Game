#pragma once
#include "GameObject.h"
#include "Client_Extend.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CPipeLine;
class CTransform;
class CNavigation;
class CMesh_Dynamic;
END

class CPlayer final :public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual HRESULT Render_GameObject();
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);

public:
	STATE Get_PlayerState() const {return m_eState; }					//플레이어의 현재 상태 반환 함수
	WEAPON Get_PlayerWeapon() const { return m_eType; }					//플레이어의 무기 상태 반환 함수
	_bool Get_IsAttacked() const { return m_bIsAttacked; }				//플레이어의 피격 여부 반환 함수
	_vec3 Get_PlayerVec() const;										//플레이어의 위치 벡터 반환 함수
	_vec3 Get_PlayerPos() const;										//플레이어의 위치 벡터 반환 함수
	_vec3 Get_Player_HeadPos() const;									//플레이어의 머리 위치 반환 함수
	_vec3 Get_Ray() const;												//플레이어의 광선 반환 함수
	_float Get_Current_Bullet() const { return m_iBulletResiduum; }		//플레이어의 탄환 잔여량 반환 함수
	_uint Get_Hp() const { return m_iCurHp; }

public:
	void Set_PlayerState(STATE eType) { m_eState = eType; }				//플레이어의 현재 상태 변경 함수
	void Set_PlayerAttacked(bool bIsMove) { m_bIsAttacked = bIsMove;}   //플레이어의 움직임 제어 bool변수 변경 함수
	void Set_Hp(_uint iDemage) { m_iCurHp -= iDemage; }					//플레이어의 체력 변경 함수

private:
	void PlayerMove(_double TimeDelta);						  //플레이어 움직임을 관리하는 함수
	void SetAnimationTIme();								  //애니메이션 셋의 period 조절 함수
	void GetWeaponCurrentBullet();							  //각 무기마다 가지고 있는 현재 총알 얻어오는 함수
	bool IsCanFire();										  //현재 총알의 개수 확인 및 공격 가능 여부 판단 함수
	void ComputeRay();


private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CMesh_Dynamic*			m_pMeshCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	CPipeLine*				m_pPipeLineCom = nullptr;

private:
	_uint					m_iMaxHP = 10000;				  //최대 체력
	_uint					m_iCurHp = 10000;				  //현재 체력

	_float					m_iBulletResiduum;				  //총알 잔여량
	_float					m_fCoolTime;					  //총알 발사 쿨타임

	_double					m_TimeDelta;					  //델타 타임
	_double					m_dAnimationTime;				  //애니메이션 period

	_bool					m_bIsAttacked = false;			  //이동 제어 변수
	_bool					m_bIsShow = false;

	_vec3					m_vRay;							  //플레이어가 쏘는 레이 벡터
	_vec3					m_vPos;							  //플레이어 위치 벡터
	_vec3					m_vMuzzle;						  //포구 위치 벡터

private:
	WEAPON					m_ePreType;						  //이전 무기 상태

	WEAPON					m_eType;						  //현재 무기 상태

	SCENEID					m_eScene;						  //현재 씬 넘버

	STATE					m_eState;						  //현재 플레이어 상태

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

	//지형을 타게 하는 함수
	HRESULT SetUp_OnTerrain();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice); // 원본을 생성한다.
	virtual CGameObject* Clone_GameObject(void* pArg); // 복제본을 생성한다.
	virtual void Free();
};
