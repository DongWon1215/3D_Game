#include "stdafx.h"
#include "..\Headers\Terrain.h"
#include "Management.h"

USING(Client)

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{

}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{

}

HRESULT CTerrain::Ready_GameObject_Prototype() // 원본객체의 초기화.
{
	/*m_pDevice->SetSamplerState(D3DSAMP_MINFILTER)*/
	return NOERROR;
}

HRESULT CTerrain::Ready_GameObject(void* pArg) // 복제된객체의 추가 초기화.
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CTerrain::Update_GameObject(_double TimeDelta)
{
	
	return _int();
}

_int CTerrain::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pVIBufferCom->Culling(m_pTransformCom, m_pFrustumCom);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CTerrain::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom ||
		nullptr == m_pNavigationCom)
		return E_FAIL;

	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	_matrix		matTransform;
	D3DXMatrixIdentity(&matTransform);
	// gARA
	_matrix			matProj;


	m_pShaderCom->Set_Value("g_matWorld", &matTransform, sizeof(_matrix));

	m_pShaderCom->Set_Value("g_matView", &pManagement->Get_Transform(D3DTS_VIEW), sizeof(_matrix));

	m_pShaderCom->Set_Value("g_matProj", &pManagement->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	m_pShaderCom->Set_Texture("g_SourTexture", m_pTextureCom->Get_Texture(1));
	m_pShaderCom->Set_Texture("g_DestTexture", m_pTextureCom->Get_Texture(1));
	m_pShaderCom->Set_Texture("g_FilterTexture", m_pFilterCom->Get_Texture(0));

	m_pShaderCom->Begin_Shader();
	m_pShaderCom->Begin_Pass(0);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Pass();
	m_pShaderCom->End_Shader();

#ifdef _DEBUG

	if (pManagement->KeyPressing(KEY_DOWN))
	{
		if (FAILED(m_pNavigationCom->Render_Navigation()))
			return E_FAIL;
	}

#endif // _DEBUG

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CTerrain::Add_Component()
{
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Terrain", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Terrain", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Filter
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Filter", L"Com_Filter", (CComponent**)&m_pFilterCom)))
		return E_FAIL;
	
	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_VIBuffer_Terrain", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Navigation
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Stage_Navigation", L"Com_Navigation", (CComponent**)&m_pNavigationCom)))
		return E_FAIL;

	// Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	return NOERROR;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTerrain* pInstance = new CTerrain(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone_GameObject(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pFilterCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pNavigationCom);

	CGameObject::Free();
}
