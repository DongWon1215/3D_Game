#include "stdafx.h"
#include "..\Headers\Wave.h"
#include "Management.h"


CWave::CWave(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CWave::CWave(const CWave & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWave::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CWave::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 200.f;
	m_fSizeY = 100.f;
	m_fStartX = g_WINCX - m_fSizeX * 0.5f;
	m_fStartY = g_WINCY - m_fSizeY * 0.5f;

	if (nullptr != pArg)
		m_iProgress = *(_uint*)pArg;

	return S_OK;
}

_int CWave::Update_GameObject(_double TimeDelta)
{
	if (m_fLifeTime <= 0.f)
		m_bIsDead = true;

	m_fLifeTime -= 1.f * TimeDelta;

	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return false;

	Safe_AddRef(pManagement);

	Safe_Release(pManagement);

	return _int();
}

_int CWave::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_fStartX - (g_WINCX >> 1), (g_WINCY >> 1) - m_fStartY, 0.f));
	m_pTransformCom->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 1.f));

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CWave::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pTransformCom->Remove_Scale();

	CPipeLine* pPipeLine = CPipeLine::GetInstance();

	D3DXFONT_DESC	tFontInfo;	// ��Ʈ����
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESC));

	tFontInfo.Width = 50;	// ��Ʈ ����
	tFontInfo.Height = 65;	// ��Ʈ ����
	tFontInfo.Weight = FW_BLACK;	// ��Ʈ ����
	tFontInfo.CharSet = HANGEUL_CHARSET;	// ���� ���
	lstrcpy(tFontInfo.FaceName, L"�ü�");	// ��ü

	LPD3DXSPRITE			pSprite = nullptr;
	if (FAILED(D3DXCreateSprite(Get_Graphic_Device(), &pSprite)))
		return E_FAIL;

	LPD3DXFONT			pFont = nullptr;
	if (FAILED(D3DXCreateFontIndirect(Get_Graphic_Device(), &tFontInfo, &pFont)))
		return E_FAIL;
	TCHAR szBuf[64] = L"";
	swprintf_s(szBuf, L" Wave %d", m_iProgress);
	RECT tempRect = { 0,0,g_WINCX,g_WINCY };

	_matrix TempMat;

	D3DXMatrixTranslation(&TempMat, 400, 80.f, 0.f);

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader();

	m_pShaderCom->Begin_Pass(0);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Pass();

	m_pShaderCom->End_Shader();

	pSprite->SetTransform(&TempMat);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	pFont->DrawText(
		pSprite, /* ��������Ʈ Com ��ü*/
		szBuf, /* ����� ���ڿ� */
		-1, /* ���ڿ� ���� */
		&tempRect, /* �簢�� ���� */
		0, /* �簢�� ���� �ȿ� �׸� ���� �ɼ� */
		D3DCOLOR_ARGB(255, 255, 255, 255) /* ��� ���� */);
	pSprite->End();

	Safe_Release(pFont);
	Safe_Release(pSprite);

	return S_OK;
}

HRESULT CWave::Add_Component()
{
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_UI", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWave::SetUp_ConstantTable()
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

	m_pShaderCom->Set_Texture("g_DiffuseTexture", m_pTextureCom->Get_Texture(1));

	Safe_Release(pManagement);

	return S_OK;
}


CWave * CWave::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWave* pInstance = new CWave(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWave::Clone_GameObject(void * pArg)
{
	CWave* pInstance = new CWave(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWave::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

	CGameObject::Free();
}