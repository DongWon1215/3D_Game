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
	STATE Get_PlayerState() const {return m_eState; }					//�÷��̾��� ���� ���� ��ȯ �Լ�
	WEAPON Get_PlayerWeapon() const { return m_eType; }					//�÷��̾��� ���� ���� ��ȯ �Լ�
	_bool Get_IsAttacked() const { return m_bIsAttacked; }				//�÷��̾��� �ǰ� ���� ��ȯ �Լ�
	_vec3 Get_PlayerVec() const;										//�÷��̾��� ��ġ ���� ��ȯ �Լ�
	_vec3 Get_PlayerPos() const;										//�÷��̾��� ��ġ ���� ��ȯ �Լ�
	_vec3 Get_Player_HeadPos() const;									//�÷��̾��� �Ӹ� ��ġ ��ȯ �Լ�
	_vec3 Get_Ray() const;												//�÷��̾��� ���� ��ȯ �Լ�
	_float Get_Current_Bullet() const { return m_iBulletResiduum; }		//�÷��̾��� źȯ �ܿ��� ��ȯ �Լ�
	_uint Get_Hp() const { return m_iCurHp; }

public:
	void Set_PlayerState(STATE eType) { m_eState = eType; }				//�÷��̾��� ���� ���� ���� �Լ�
	void Set_PlayerAttacked(bool bIsMove) { m_bIsAttacked = bIsMove;}   //�÷��̾��� ������ ���� bool���� ���� �Լ�
	void Set_Hp(_uint iDemage) { m_iCurHp -= iDemage; }					//�÷��̾��� ü�� ���� �Լ�

private:
	void PlayerMove(_double TimeDelta);						  //�÷��̾� �������� �����ϴ� �Լ�
	void SetAnimationTIme();								  //�ִϸ��̼� ���� period ���� �Լ�
	void GetWeaponCurrentBullet();							  //�� ���⸶�� ������ �ִ� ���� �Ѿ� ������ �Լ�
	bool IsCanFire();										  //���� �Ѿ��� ���� Ȯ�� �� ���� ���� ���� �Ǵ� �Լ�
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
	_uint					m_iMaxHP = 10000;				  //�ִ� ü��
	_uint					m_iCurHp = 10000;				  //���� ü��

	_float					m_iBulletResiduum;				  //�Ѿ� �ܿ���
	_float					m_fCoolTime;					  //�Ѿ� �߻� ��Ÿ��

	_double					m_TimeDelta;					  //��Ÿ Ÿ��
	_double					m_dAnimationTime;				  //�ִϸ��̼� period

	_bool					m_bIsAttacked = false;			  //�̵� ���� ����
	_bool					m_bIsShow = false;

	_vec3					m_vRay;							  //�÷��̾ ��� ���� ����
	_vec3					m_vPos;							  //�÷��̾� ��ġ ����
	_vec3					m_vMuzzle;						  //���� ��ġ ����

private:
	WEAPON					m_ePreType;						  //���� ���� ����

	WEAPON					m_eType;						  //���� ���� ����

	SCENEID					m_eScene;						  //���� �� �ѹ�

	STATE					m_eState;						  //���� �÷��̾� ����

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

	//������ Ÿ�� �ϴ� �Լ�
	HRESULT SetUp_OnTerrain();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice); // ������ �����Ѵ�.
	virtual CGameObject* Clone_GameObject(void* pArg); // �������� �����Ѵ�.
	virtual void Free();
};
