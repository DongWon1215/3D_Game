#include "stdafx.h"
#include "..\Headers\BackGround.h"
#include "Management.h"

USING(Client)

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CBackGround::CBackGround(const CBackGround & rhs)
	:CGameObject(rhs)
{
}

HRESULT CBackGround::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_iIndex = rand() % 3;

	

	return S_OK;
}

_int CBackGround::Update_GameObject(_double TimeDelta)
{
	return _int();
}

_int CBackGround::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_PRIORITY, this);

	return _int();
}

HRESULT CBackGround::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	_matrix		matTransform;
	D3DXMatrixIdentity(&matTransform);

	m_pShaderCom->Set_Value("g_matWorld", &matTransform, sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", &matTransform, sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", &matTransform, sizeof(_matrix));

	m_pShaderCom->Set_Texture("g_DiffuseTexture", m_pTextureCom->Get_Texture(m_iIndex));

	m_pShaderCom->Begin_Shader();
	m_pShaderCom->Begin_Pass(1);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Pass();
	m_pShaderCom->End_Shader();
	return S_OK;
}

HRESULT CBackGround::Add_Component()
{
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Default", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBackGround* pInstance = new CBackGround(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGround::Clone_GameObject(void * pArg)
{
	CGameObject* pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

	CGameObject::Free();
}
