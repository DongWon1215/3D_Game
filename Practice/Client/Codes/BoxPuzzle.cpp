#include "stdafx.h"
#include "..\Headers\BoxPuzzle.h"
#include "Management.h"
#include "Player.h"

CBoxPuzzle::CBoxPuzzle(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CBoxPuzzle::CBoxPuzzle(const CBoxPuzzle & rhs)
	:CGameObject(rhs)
{
}

HRESULT CBoxPuzzle::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBoxPuzzle::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_iStartX = g_WINCX;
	m_iStartY = g_WINCY;

	ReSet();

	return S_OK;
}

_int CBoxPuzzle::Update_GameObject(_double TimeDelta)
{
	if (m_bIsShow)
	{
		FindTresure();
	}
	else ReSet();

	if (m_bIsClear)
	{
		m_bIsShow = false;
		Reward();
		m_bIsDead = true;
	}

	if (m_iLife == 0)
		m_bIsShow = false;

	return _int();
}

_int CBoxPuzzle::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_bIsShow)
	{
		if (nullptr == m_pRendererCom)
			return -1;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_iStartX, m_iStartY, 0.f));
		m_pTransformCom->Set_Scale(_vec3(1200.f, 300.f, 1.f));
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);
	}
	return _int();
}

HRESULT CBoxPuzzle::Render_GameObject()
{
	LPD3DXSPRITE			pSprite = nullptr;
	if (FAILED(D3DXCreateSprite(Get_Graphic_Device(), &pSprite)))
		return E_FAIL;
	if (m_bIsShow)
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
	}
	return S_OK;
}

HRESULT CBoxPuzzle::FindTresure()
{

	POINT		ptMouse;

	GetCursorPos(&ptMouse);

	for (int i = 0; i < 100; i++)
	{
		if (PtInRect(&m_RcBox[i].rc, ptMouse) && m_RcBox[i].bIsTresure)
			m_bIsClear = true;

		else if (PtInRect(&m_RcBox[i].rc, ptMouse) && !m_RcBox[i].bIsTresure)
			m_iLife--;
	}

	return S_OK;
}

HRESULT CBoxPuzzle::Add_Component()
{// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Circle", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoxPuzzle::SetUp_ConstantTable()
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

HRESULT CBoxPuzzle::ReSet()
{

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_RcBox[i * 10 + j].vecPos = { 20.f * (i * 10 + j), 20.f * (i * 10 + j),0.f };
			m_RcBox[i * 10 + j].rc = { LONG(m_RcBox[i * 10 + j].vecPos.x) - 10, LONG(m_RcBox[i * 10 + j].vecPos.y) - 10, LONG(m_RcBox[i * 10 + j].vecPos.x) + 10, LONG(m_RcBox[i * 10 + j].vecPos.y) + 10 };
			m_RcBox[i * 10 + j].bIsTresure = false;
		}
	}

	m_RcBox[rand() % 100].bIsTresure = true;

	return S_OK;
}

void CBoxPuzzle::Reward()
{
	CManagement*	pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
	{
		MSG_BOX("Error From Reward");
		return;
	}

	Safe_AddRef(pManagement);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(L"GameObject_Player", SCENE_STAGE, L"Layer_Player", 0));

	if (nullptr == pPlayer)
		return;
	Safe_AddRef(pPlayer);
	pPlayer->Set_Hp(30);

	Safe_Release(pPlayer);

	Safe_Release(pManagement);

}

CBoxPuzzle * CBoxPuzzle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoxPuzzle* pInstance = new CBoxPuzzle(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CBoxPuzzle");
		Safe_Release(pInstance);
	}

	return pInstance;
	return nullptr;
}

CGameObject * CBoxPuzzle::Clone_GameObject(void * pArg)
{
	CBoxPuzzle* pInstance = new CBoxPuzzle(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CBoxPuzzle");
		Safe_Release(pInstance);
	}

	return nullptr;
}

void CBoxPuzzle::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

	CGameObject::Free();
}
