#pragma once
#include "Component.h"

BEGIN(Engine)
class CShader;
class CPipeLine;
class CTransform;
class ENGINE_DLL CCollider final : public CComponent
{
public:
	typedef struct tagColliderDesc
	{
		_matrix*	pTargetMatrix;
		_vec3		vLocalScale, vLocalPosition;
		_float		fRadius;
	}COLLIDER_DESC;

	typedef struct tagOBBDesc
	{
		_vec3	vAlignAxis[3],
				vProjAxis[3],
				vCenterPos;
	}OBB_DESC;

public:
	enum COLLIDERTYPE {TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END
	};

private:
	explicit CCollider(LPDIRECT3DDEVICE9 pDevice);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Ready_Component_Prototype(COLLIDERTYPE eType);
	virtual HRESULT Ready_Component(void* pArg);

public:
	_int Update_Collider(_double TimeDelta);
	_bool Collision_AABB(CCollider* pTargetCollider);
	_bool Collision_OBB(CCollider* pTargetCollider);
	_bool Collision_Sphere(CCollider* pTargetCollider);
	_bool Collision_Ray(_vec3* vRay, _vec3* vPosition);

#ifdef _DEBUG
public:
	HRESULT Render_Collider();
#endif
private:
	LPD3DXMESH			m_pMesh = nullptr;
	COLLIDERTYPE		m_eType;
	COLLIDER_DESC		m_Collider_Desc;

private:
	CShader*			m_pShader = nullptr;
	CTransform*			m_pTransform = nullptr;
	CPipeLine*			m_pPipeLine = nullptr;

private:
	OBB_DESC*			m_pOBB_Desc = nullptr;
	_vec3				m_vMin, m_vMax;
	_bool				m_isCollision = false;

private:
	HRESULT Ready_BoundingBox();
	HRESULT Ready_BoundingSphere();

	// CComponent을(를) 통해 상속됨
public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pDevice, COLLIDERTYPE eType);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free();
};

END