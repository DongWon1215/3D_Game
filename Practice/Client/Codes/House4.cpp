#include "stdafx.h"
#include "Management.h"
#include "..\Headers\House4.h"

USING(Client)

CHouse4::CHouse4(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CHouse4::CHouse4(const CHouse4 & rhs)
	: CGameObject(rhs)
{

}

HRESULT CHouse4::Ready_GameObject_Prototype() // 원본객체의 초기화.
{

	return S_OK;
}

HRESULT CHouse4::Ready_GameObject(void* pArg) // 복제된객체의 추가 초기화.
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_matrix worldmat = *(_matrix*)pArg;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (_vec3)worldmat.m[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, (_vec3)worldmat.m[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, (_vec3)worldmat.m[2]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (_vec3)worldmat.m[3]);

	
	return S_OK;
}

_int CHouse4::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	SetUp_OnTerrain();
	return _int();
}

_int CHouse4::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);
	
	return _int();
}

HRESULT CHouse4::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pMeshCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;
	
	m_pShaderCom->Begin_Shader();
	m_pShaderCom->Begin_Pass(0);


	_ulong dwNumSubset = m_pMeshCom->Get_NumSubset();

	for (_ulong i = 0; i < dwNumSubset; ++i)
	{
		if (FAILED(m_pShaderCom->Set_Texture("g_DiffuseTexture", m_pMeshCom->Get_MaterialTexture(i, MESHTEXTURE::TYPE_DIFFUSE))))
			return E_FAIL;		

		m_pShaderCom->Commit_Change();

		m_pMeshCom->Render_Mesh(i);
	}

	m_pShaderCom->End_Pass();
	m_pShaderCom->End_Shader();

	return S_OK;
}

HRESULT CHouse4::Add_Component()
{
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Mesh_House4", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;


	

	// For.Com_Transform
	CTransform::STATEDESC		StateDesc;
	StateDesc.fSpeedPerSec = 5.f;
	StateDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &StateDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CHouse4::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pTransformCom)
		return E_FAIL;

	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Value("g_matView", &pManagement->Get_Transform(D3DTS_VIEW), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Value("g_matProj", &pManagement->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix))))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CHouse4::SetUp_OnTerrain()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	const CVIBuffer_Terrain* pTerrainBuffer = (const CVIBuffer_Terrain*)pManagement->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
	{
		Safe_Release(pManagement);
		return -1;
	}

	_vec3		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = pTerrainBuffer->Compute_Y(vPosition);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	Safe_Release(pManagement);

	return S_OK;
}

CHouse4 * CHouse4::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHouse4* pInstance = new CHouse4(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CHouse4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHouse4::Clone_GameObject(void* pArg)
{
	CHouse4* pInstance = new CHouse4(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CHouse4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHouse4::Free()
{
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
