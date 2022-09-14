#include "stdafx.h"
#include "Object_Tool.h"
#include "Management.h"
#include "Map_Object.h"

CObject_Tool::CObject_Tool(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CObject_Tool::CObject_Tool(CObject_Tool & rhs)
	:CGameObject(rhs)
{
}

HRESULT CObject_Tool::Ready_GameObject_Prototype()
{

	return S_OK;
}

HRESULT CObject_Tool::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		CMap_Object::OBJECT pObject;
		memcpy(&pObject, (CMap_Object::OBJECT*)pArg, sizeof(CMap_Object::OBJECT));
		
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (_vec3)pObject.WorldMatrix.m[0]);
		m_pTransformCom->Set_State(CTransform::STATE_UP, (_vec3)pObject.WorldMatrix.m[1]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, (_vec3)pObject.WorldMatrix.m[2]);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (_vec3)pObject.WorldMatrix.m[3]);
		
		
		if (FAILED(CGameObject::Add_Component(0, pObject.TextInfo.ProtoTypeName, L"Com_Mesh", (CComponent**)&m_pMeshCom)))
			return E_FAIL;
	}

	return S_OK;
}

_int CObject_Tool::Update_GameObject(_double TimeDelta)
{
	CGameObject::Update_GameObject(TimeDelta);
	
	SetUp_OnTerrain();

	return _int();
}

_int CObject_Tool::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CObject_Tool::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pMeshCom)
		return E_FAIL;

	*this;

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

HRESULT CObject_Tool::Add_Component()
{
	*this;

	if (FAILED(CGameObject::Add_Component(0, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(0, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	CTransform::STATEDESC Statedesc;

	Statedesc.fSpeedPerSec = 5.f;
	Statedesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(0, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &Statedesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CObject_Tool::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom)
		return E_FAIL;

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement) return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Value("g_matView", &pManagement->Get_Transform(D3DTS_VIEW), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Value("g_matProj", &pManagement->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix))))
		return E_FAIL;

	D3DLIGHT9 LightDesc = pManagement->Get_LightDesc(0);

	m_pShaderCom->Set_Value("g_vLightDir", &LightDesc.Direction, sizeof(_vec4));
	m_pShaderCom->Set_Value("g_vLightDiffuse", &LightDesc.Diffuse, sizeof(_vec4));
	m_pShaderCom->Set_Value("g_vLightAmbient", &LightDesc.Ambient, sizeof(_vec4));
	m_pShaderCom->Set_Value("g_vLightSpecular", &LightDesc.Specular, sizeof(_vec4));

	_matrix ViewMatrix = pManagement->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pShaderCom->Set_Value("g_vCamPosition", &ViewMatrix.m[3][0], sizeof(_vec4));

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CObject_Tool::SetUp_OnTerrain()
{
	return S_OK;
}

CObject_Tool * CObject_Tool::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CObject_Tool* pInstance = new CObject_Tool(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CObject_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Tool::Free()
{
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

CGameObject * CObject_Tool::Clone_GameObject(void * pArg)
{
	CObject_Tool* pInstance = new CObject_Tool(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CObject_Tool_Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}