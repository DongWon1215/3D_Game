#include "stdafx.h"
#include "..\Headers\HPBar.h"
#include "Management.h"
#include "Player.h"

CHPBar::CHPBar(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CHPBar::CHPBar(const CHPBar & rhs)
	:CGameObject(rhs)
{
}

HRESULT CHPBar::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CHPBar::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fMaxGauge = 10000.f;

	return S_OK;
}

_int CHPBar::Update_GameObject(_double TimeDelta)
{

	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return false;

	Safe_AddRef(pManagement);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(L"GameObject_Player", SCENE_STAGE, L"Layer_Player", 0));

	m_fCurGauge = pPlayer->Get_Hp() / m_fMaxGauge * 100.f;

	Safe_Release(pManagement);

	return _int();
}

_int CHPBar::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3((g_WINCX - m_fCurGauge * 1.f) - (g_WINCX >> 1), (g_WINCY >> 1) - (g_WINCY - 200 * 0.5f), 0.f));
	m_pTransformCom->Set_Scale(_vec3(m_fCurGauge * 2.f, 50.f, 1.f));
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CHPBar::Render_GameObject()
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

HRESULT CHPBar::Add_Component()
{
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_HPBar", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHPBar::SetUp_ConstantTable()
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

CHPBar * CHPBar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CHPBar* pInstance = new CHPBar(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHPBar::Clone_GameObject(void * pArg)
{
	CHPBar* pInstance = new CHPBar(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHPBar::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

	CGameObject::Free();
}
