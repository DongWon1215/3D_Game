#include "..\Headers\Collider.h"
#include "..\Headers\PipeLine.h"
#include "..\Headers\Shader.h"
#include "..\Headers\Transform.h"


CCollider::CCollider(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_isCollision(false)
{
	Safe_AddRef(m_pPipeLine);
}

CCollider::CCollider(const CCollider& rhs)
	: CComponent(rhs)
	, m_eType(rhs.m_eType)
	, m_pShader(rhs.m_pShader)
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_isCollision(rhs.m_isCollision)
{
	if (FAILED(rhs.m_pMesh->CloneMeshFVF(rhs.m_pMesh->GetOptions(), rhs.m_pMesh->GetFVF(), Get_Graphic_Device(), &m_pMesh)))
		MSG_BOX("Failed To Clone Collider Mesh");

	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pShader);
}

//충돌 박스를 생성하는 함수
HRESULT CCollider::Ready_BoundingBox()
{
	if (FAILED(D3DXCreateBox(Get_Graphic_Device(), 1.f, 1.f, 1.f, &m_pMesh, nullptr)))
		return E_FAIL;

	return S_OK;
}

//충돌 구를 생성하는 함수
HRESULT CCollider::Ready_BoundingSphere()
{
	if (FAILED(D3DXCreateSphere(Get_Graphic_Device(), 0.5f, 20, 20, &m_pMesh, nullptr)))
		return E_FAIL;

	return S_OK;
}

//콜리더 프로토 타입을 생성하는 함수
HRESULT CCollider::Ready_Component_Prototype(COLLIDERTYPE eType)
{
	m_eType = eType;

	HRESULT hr = 0;

	switch (eType)
	{
	case CCollider::TYPE_AABB : case CCollider::TYPE_OBB:
		hr = Ready_BoundingBox();
		break;

	case CCollider::TYPE_SPHERE:
		hr = Ready_BoundingSphere();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	m_pShader = CShader::Create(Get_Graphic_Device(), L"../Bin/Shader/Shader_Collider.fx");
	if (nullptr == m_pShader)
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider::Ready_Component(void * pArg)
{
	m_pTransform = CTransform::Create(Get_Graphic_Device());
	if (nullptr == m_pTransform)
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_Collider_Desc, pArg, sizeof(COLLIDER_DESC));

	_matrix matTransform;

	_matrix matScale;

	switch (m_eType)
	{
	case CCollider::TYPE_AABB:
	case CCollider::TYPE_OBB:
		D3DXMatrixScaling(&matScale, m_Collider_Desc.vLocalScale.x, m_Collider_Desc.vLocalScale.y, m_Collider_Desc.vLocalScale.z);
		break;
		
	case CCollider::TYPE_SPHERE:
		D3DXMatrixScaling(&matScale, m_Collider_Desc.fRadius, m_Collider_Desc.fRadius, m_Collider_Desc.fRadius);
		break;
	}

	_matrix matPosition;
	D3DXMatrixTranslation(&matPosition, m_Collider_Desc.vLocalPosition.x, m_Collider_Desc.vLocalPosition.y, m_Collider_Desc.vLocalPosition.z);
	
	matTransform = matScale * matPosition;

	_ulong dwNumVertices = m_pMesh->GetNumVertices();
	_uint iStride = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	void* pVertices = nullptr;

	m_pMesh->LockVertexBuffer(0, &pVertices);

	for (size_t i = 0; i < dwNumVertices; i++)
		D3DXVec3TransformCoord((_vec3*)((_byte*)pVertices + i * iStride), (_vec3*)((_byte*)pVertices + i * iStride), &matTransform);

	if (FAILED(D3DXComputeBoundingBox((_vec3*)pVertices,dwNumVertices,iStride,&m_vMin,&m_vMax)))
		return E_FAIL;

	m_pMesh->UnlockVertexBuffer();

	if (TYPE_SPHERE != m_eType)
	{
		m_pOBB_Desc = new OBB_DESC;
		ZeroMemory(m_pOBB_Desc, sizeof(OBB_DESC));

		_vec3 vPoint[8];

		vPoint[0] = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
		vPoint[1] = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
		vPoint[2] = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
		vPoint[3] = m_vMin;

		vPoint[4] = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
		vPoint[5] = m_vMax;
		vPoint[6] = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
		vPoint[7] = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);

		m_pOBB_Desc->vCenterPos = (vPoint[3] + vPoint[5]) * 0.5f;

		m_pOBB_Desc->vAlignAxis[0] = vPoint[2] - vPoint[3];
		m_pOBB_Desc->vAlignAxis[1] = vPoint[0] - vPoint[3];
		m_pOBB_Desc->vAlignAxis[2] = vPoint[7] - vPoint[3];

		m_pOBB_Desc->vProjAxis[0] = (vPoint[5] + vPoint[2]) * 0.5f - m_pOBB_Desc->vCenterPos;
		m_pOBB_Desc->vProjAxis[1] = (vPoint[5] + vPoint[0]) * 0.5f - m_pOBB_Desc->vCenterPos;
		m_pOBB_Desc->vProjAxis[2] = (vPoint[5] + vPoint[7]) * 0.5f - m_pOBB_Desc->vCenterPos;
	}


	return S_OK;
}

_int CCollider::Update_Collider(_double TimeDelta)
{
	switch (m_eType)
	{
	case CCollider::TYPE_AABB:
		m_pTransform->Set_State(CTransform::STATE_POSITION, *(_vec3*)&m_Collider_Desc.pTargetMatrix->m[3][0]);
		break;

	case CCollider::TYPE_OBB:
		m_pTransform->Set_State(CTransform::STATE_RIGHT, *(_vec3*)&m_Collider_Desc.pTargetMatrix->m[0][0]);
		m_pTransform->Set_State(CTransform::STATE_UP, *(_vec3*)&m_Collider_Desc.pTargetMatrix->m[1][0]);
		m_pTransform->Set_State(CTransform::STATE_LOOK, *(_vec3*)&m_Collider_Desc.pTargetMatrix->m[2][0]);
		m_pTransform->Set_State(CTransform::STATE_POSITION, *(_vec3*)&m_Collider_Desc.pTargetMatrix->m[3][0]);
		break;

	case CCollider::TYPE_SPHERE:
		m_pTransform->Set_State(CTransform::STATE_POSITION, *(_vec3*)&m_Collider_Desc.pTargetMatrix->m[3][0]);
		break;
	}

	return _int();
}

_bool CCollider::Collision_AABB(CCollider * pTargetCollider)
{
	if (nullptr == pTargetCollider)
		return false;

	_vec3 vSourMin, vSourMax, vDestMin, vDestMax;

	D3DXVec3TransformCoord(&vSourMin, &m_vMin, m_pTransform->Get_WorldMatrixPointer());
	D3DXVec3TransformCoord(&vSourMax, &m_vMax, m_pTransform->Get_WorldMatrixPointer());

	D3DXVec3TransformCoord(&vDestMin, &pTargetCollider->m_vMin, pTargetCollider->m_pTransform->Get_WorldMatrixPointer());
	D3DXVec3TransformCoord(&vDestMax, &pTargetCollider->m_vMax, pTargetCollider->m_pTransform->Get_WorldMatrixPointer());

	m_isCollision = false;

	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		return false;

	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return false;

	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		return false;

	m_isCollision = true;

	return true;
}

_bool CCollider::Collision_OBB(CCollider * pTargetCollider)
{
	OBB_DESC OBBDesc[2];

	_matrix matWorld[2] = { {m_pTransform->Get_WorldMatrix()}, {pTargetCollider->m_pTransform->Get_WorldMatrix()}};

	memcpy(&OBBDesc[0], m_pOBB_Desc, sizeof(OBB_DESC));
	memcpy(&OBBDesc[1], pTargetCollider->m_pOBB_Desc, sizeof(OBB_DESC));

	for (size_t i = 0; i < 2; i++)
	{
		D3DXVec3TransformCoord(&OBBDesc[i].vCenterPos,&OBBDesc[i].vCenterPos,&matWorld[i]);
		for (size_t j = 0; j < 3; j++)
		{
			D3DXVec3TransformNormal(&OBBDesc[i].vProjAxis[j], &OBBDesc[i].vProjAxis[j], &matWorld[i]);	
			D3DXVec3TransformNormal(&OBBDesc[i].vAlignAxis[j], &OBBDesc[i].vAlignAxis[j], &matWorld[i]);
			D3DXVec3Normalize(&OBBDesc[i].vProjAxis[j], &OBBDesc[i].vProjAxis[j]);
		}
	}

	_float fDistance[3];

	m_isCollision = false;

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			_vec3 vDir = OBBDesc[1].vCenterPos - OBBDesc[0].vCenterPos;
			fDistance[0] = fabs(D3DXVec3Dot(&vDir, &OBBDesc[i].vAlignAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&OBBDesc[0].vProjAxis[0], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[0].vProjAxis[1], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[0].vProjAxis[2], &OBBDesc[i].vAlignAxis[j]));

			fDistance[2] = fabs(D3DXVec3Dot(&OBBDesc[1].vProjAxis[0], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[1].vProjAxis[1], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[1].vProjAxis[2], &OBBDesc[i].vAlignAxis[j]));

			if (fDistance[0] > fDistance[1] + fDistance[2])
				return false;
		}
	}

	m_isCollision = true;

	return true;
}

_bool CCollider::Collision_Sphere(CCollider * pTargetCollider)
{
	if (nullptr == pTargetCollider)
		return false;

	m_isCollision = false;

	if (D3DXVec3Length(&(pTargetCollider->m_pTransform->Get_State(CTransform::STATE_POSITION) - m_pTransform->Get_State(CTransform::STATE_POSITION))) > m_Collider_Desc.fRadius)
		return false;

	return m_isCollision = true;
}

_bool CCollider::Collision_Ray(_vec3* vRay, _vec3* vPosition)
{
	//D3DXVec3Normalize(vRay, vRay);
	D3DXVec3TransformNormal(vRay, vRay, &m_pTransform->Get_WorldMatrixInverse());
	D3DXVec3TransformCoord(vPosition,vPosition ,&m_pTransform->Get_WorldMatrixInverse());

	_vec3 Point[8] =
	{
		{ m_vMin.x, m_vMax.y, m_vMin.z },
		{ m_vMax.x, m_vMax.y, m_vMin.z },
		{ m_vMax.x, m_vMin.y, m_vMin.z },
		{ m_vMin.x, m_vMin.y, m_vMin.z },
		{ m_vMin.x, m_vMax.y, m_vMax.z },
		{ m_vMax.x, m_vMax.y, m_vMax.z },
		{ m_vMax.x, m_vMin.y, m_vMax.z },
		{ m_vMin.x, m_vMin.y, m_vMax.z }
	};
	float u = 0.f, v = 0.f, dist = 0.f;

	switch (m_eType)
	{
	case TYPE_AABB:
	case TYPE_OBB:

		

		//앞면
		if (D3DXIntersectTri(&Point[0], &Point[1], &Point[2], vPosition, vRay, &u, &v, &dist))
			return true;
		if (D3DXIntersectTri(&Point[0], &Point[2], &Point[3], vPosition, vRay, &u, &v, &dist))
			return true;

		//왼쪽면
		if (D3DXIntersectTri(&Point[4], &Point[0], &Point[3], vPosition, vRay, &u, &v, &dist))
			return true;
		if (D3DXIntersectTri(&Point[4], &Point[3], &Point[7], vPosition, vRay, &u, &v, &dist))
			return true;

		//오른쪽면
		if (D3DXIntersectTri(&Point[1], &Point[5], &Point[6], vPosition, vRay, &u, &v, &dist))
			return true;
		if (D3DXIntersectTri(&Point[1], &Point[6], &Point[2], vPosition, vRay, &u, &v, &dist))
			return true;

		//뒷면
		if (D3DXIntersectTri(&Point[5], &Point[4], &Point[7], vPosition, vRay, &u, &v, &dist))
			return true;
		if (D3DXIntersectTri(&Point[5], &Point[7], &Point[6], vPosition, vRay, &u, &v, &dist))
			return true;

		return false;

	case TYPE_SPHERE:

		//_vec3 SphereVec = m_pTransform->Get_State(CTransform::STATE_POSITION) - *vPosition;
		//_double radian = D3DXVec3Dot(&SphereVec, vRay);
		//_double SphereVecSquared = D3DXVec3Dot(&SphereVec, vRay);
		//_double radiusSquared = pow(m_Collider_Desc.fRadius, 2);
		//
		//if (radian < 0 || radiusSquared < SphereVecSquared)
		//	return false;
		//
		//_double CheckRay = SphereVecSquared - pow(radian, 2);
		//
		//if (CheckRay > radiusSquared)
		//	return false;

		BOOL IsCollision;
		DWORD Index;
		if (D3DXIntersect(m_pMesh, vPosition, vRay, &IsCollision, &Index, &u, &v, &dist, nullptr, nullptr))
			return true; 

		return false;
	}
}

#ifdef _DEBUG
HRESULT CCollider::Render_Collider()
{
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pShader->Set_Value("g_matWorld", &m_pTransform->Get_WorldMatrix(), sizeof(_matrix));
	m_pShader->Set_Value("g_matView", &m_pPipeLine->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShader->Set_Value("g_matProj", &m_pPipeLine->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	m_pShader->Set_Bool("g_isCollision", m_isCollision);

	m_pShader->Begin_Shader();
	m_pShader->Begin_Pass(0);

	m_pMesh->DrawSubset(0);

	m_pShader->End_Pass();
	m_pShader->End_Shader();

	m_pMesh->DrawSubset(0);

	return S_OK;
}
#endif

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pDevice, COLLIDERTYPE eType)
{
	CCollider* pInstance = new CCollider(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype(eType)))
	{
		MSG_BOX("Failed while Creating CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone_Component(void * pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed while Creating CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pShader);
	Safe_Release(m_pMesh);
	CComponent::Free();
}