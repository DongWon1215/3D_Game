#include "stdafx.h"
#include "..\Headers\HitScreen.h"
#include "Management.h"

CHitScreen::CHitScreen(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CHitScreen::CHitScreen(const CHitScreen & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHitScreen::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CHitScreen::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fLifeTime = 5.f;

	m_vPosition = _vec3(rand() % 400 - 200, rand() % 600 - 300, 0.f);
	m_pTransformCom->Set_Rotation(&_vec3(0.f, 0.f, 1.f), D3DXToRadian(rand() % 180));
	//m_pTransformCom->Set_Scale(_vec3(2.f, 2.f, 2.f));

	return S_OK;
}

_int CHitScreen::Update_GameObject(_double TimeDelta)
{
	if (m_fLifeTime <= 0)
		m_bIsDead = true;

	m_fLifeTime -= 2.5f * TimeDelta;

	return _int();
}

_int CHitScreen::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPosition);
	m_pTransformCom->Set_Scale(_vec3(1200.f, 300.f, 1.f));
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CHitScreen::Render_GameObject()
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

HRESULT CHitScreen::Add_Component()
{
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_HitScreen", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CHitScreen::SetUp_ConstantTable()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	_matrix		matTransform;
	D3DXMatrixIdentity(&matTransform);

	_matrix			ProjMatrix;
	D3DXMatrixOrthoLH(&ProjMatrix, g_WINCX, g_WINCY, 0.f, 1.f);

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", &matTransform, sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", &ProjMatrix, sizeof(_matrix));

	m_pShaderCom->Set_Texture("g_DiffuseTexture", m_pTextureCom->Get_Texture(0));

	Safe_Release(pManagement);

	return S_OK;
}

CHitScreen * CHitScreen::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CHitScreen* pInstance = new CHitScreen(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CGameObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHitScreen::Clone_GameObject(void * pArg)
{
	CHitScreen* pInstance = new CHitScreen(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CGameObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHitScreen::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
