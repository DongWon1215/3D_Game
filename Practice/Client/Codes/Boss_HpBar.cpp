#include "stdafx.h"
#include "..\Headers\BossHpBar.h"
#include "Management.h"
#include "Boss.h"

CBossHpBar::CBossHpBar(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CBossHpBar::CBossHpBar(const CBossHpBar & rhs)
	:CGameObject(rhs)
{
}

HRESULT CBossHpBar::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBossHpBar::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fMaxGauge = 1000.f;

	return S_OK;
}

_int CBossHpBar::Update_GameObject(_double TimeDelta)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return false;

	Safe_AddRef(pManagement);

	CBoss* pBoss = dynamic_cast<CBoss*>(pManagement->Get_GameObject(L"GameObject_Boss", SCENE_STAGE, L"Layer_Monster", 0));

	if (nullptr == pBoss)
		m_bIsDead = true;

	m_fCurGauge = pBoss->Get_Hp() / m_fMaxGauge * 100.f;

	Safe_Release(pManagement);

	return _int();
}

_int CBossHpBar::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(((g_WINCX >> 1) - m_fCurGauge * 1.f) - (g_WINCX >> 1), (g_WINCY >> 1) - 25.f, 0.f));
	m_pTransformCom->Set_Scale(_vec3(m_fCurGauge * 10.f , 50.f, 1.f));
	
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CBossHpBar::Render_GameObject()
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

HRESULT CBossHpBar::Add_Component()
{
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_BossHPBar", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossHpBar::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom)
		return E_FAIL;

	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	_matrix		matTransform;
	D3DXMatrixIdentity(&matTransform);

	_matrix			ProjMatrix;
	D3DXMatrixOrthoLH(&ProjMatrix, g_WINCX, g_WINCY, 0.f, 1.f);

	if (FAILED(m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Value("g_matView", &matTransform, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Value("g_matProj", &ProjMatrix, sizeof(_matrix))))
		return E_FAIL;

	m_pShaderCom->Set_Texture("g_DiffuseTexture", m_pTextureCom->Get_Texture(0));

	Safe_Release(pManagement);

	return S_OK;
}

CBossHpBar * CBossHpBar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBossHpBar* pInstance = new CBossHpBar(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CBossHpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBossHpBar::Clone_GameObject(void * pArg)
{
	CBossHpBar* pInstance = new CBossHpBar(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CBossHpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossHpBar::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

	CGameObject::Free();
}
