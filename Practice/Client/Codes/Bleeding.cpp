#include "stdafx.h"
#include "..\Headers\Bleeding.h"
#include "Management.h"


CBleeding::CBleeding(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CBleeding::CBleeding(const CBleeding & rhs)
	:CGameObject(rhs)
{
}

HRESULT CBleeding::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBleeding::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		_vec3 pPos = *(_vec3*)pArg;
		m_pTransformCom->Set_Scale(_vec3(1.f, 1.f, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pPos);
	}

	return S_OK;
}

_int CBleeding::Update_GameObject(_double TimeDelta)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return false;

	Safe_AddRef(pManagement);

	if (m_fFrame >= 15.f)
		m_bIsDead = true;

	m_fFrame += 16.f * TimeDelta;

	Safe_Release(pManagement);

	return _int();
}

_int CBleeding::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	BillBoard();
	
	Compute_ViewZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CBleeding::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader();

	m_pShaderCom->Begin_Pass(0);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Pass();

	m_pShaderCom->End_Shader();

	return S_OK;
}

HRESULT CBleeding::BillBoard()
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	_matrix TempMat = pManagement->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&TempMat, nullptr, &TempMat);

	_vec3 Look, Right;
	Right = TempMat.m[0];
	Look = TempMat.m[2];

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, Right * m_pTransformCom->Get_Scale(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, Look * m_pTransformCom->Get_Scale(CTransform::STATE_LOOK));

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CBleeding::Add_Component()
{// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Bleeding", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBleeding::SetUp_ConstantTable()
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

	m_pShaderCom->Set_Texture("g_DiffuseTexture", m_pTextureCom->Get_Texture(m_fFrame));

	Safe_Release(pManagement);

	return S_OK;
}

CBleeding * CBleeding::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBleeding* pInstance = new CBleeding(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CBleeding");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBleeding::Clone_GameObject(void * pArg)
{
	CBleeding* pInstance = new CBleeding(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CBleeding");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBleeding::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

	CGameObject::Free();
}
