#include "stdafx.h"
#include "..\Headers\BulletResiduum.h"
#include "Management.h"
#include "Player.h"
#include "PipeLine.h"

CBulletResiduum::CBulletResiduum(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CBulletResiduum::CBulletResiduum(const CBulletResiduum & rhs)
	:CGameObject(rhs)
{
}

HRESULT CBulletResiduum::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBulletResiduum::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 200.f;
	m_fSizeY = 50.f;
	m_fStartX = g_WINCX - m_fSizeX * 0.5f;
	m_fStartY = g_WINCY - m_fSizeY * 0.5f;

	return S_OK;
}

_int CBulletResiduum::Update_GameObject(_double TimeDelta)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return false;

	Safe_AddRef(pManagement);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(L"GameObject_Player", SCENE_STAGE, L"Layer_Player", 0));
	m_ePlayerWeapon = pPlayer->Get_PlayerWeapon();

	switch (m_ePlayerWeapon)
	{
	case WEAPON_PISTOL:
		m_iMaxBullet = 15;
		break;

	case WEAPON_RIFFLE:
		m_iMaxBullet = 30;
		break;

	case WEAPON_SNIPER:
		m_iMaxBullet = 5;
		break;

	}

	m_iCurBullet = pPlayer->Get_Current_Bullet();

	Safe_Release(pManagement);

	return _int();
}

_int CBulletResiduum::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_fStartX - (g_WINCX >> 1), (g_WINCY >> 1) - m_fStartY, 0.f));
	m_pTransformCom->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 1.f));

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CBulletResiduum::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pTransformCom->Remove_Scale();

	CPipeLine* pPipeLine = CPipeLine::GetInstance();

	D3DXFONT_DESC	tFontInfo;	// 폰트정보
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESC));
	
	tFontInfo.Width = 15;	// 폰트 가로
	tFontInfo.Height = 20;	// 폰트 세로
	tFontInfo.Weight = FW_BLACK;	// 폰트 굵기
	tFontInfo.CharSet = HANGEUL_CHARSET;	// 지원 언어
	lstrcpy(tFontInfo.FaceName, L"궁서");	// 서체
	
	LPD3DXSPRITE			pSprite = nullptr;
	if (FAILED(D3DXCreateSprite(Get_Graphic_Device(), &pSprite)))
		return E_FAIL;
	
	LPD3DXFONT			pFont = nullptr;
	if (FAILED(D3DXCreateFontIndirect(Get_Graphic_Device(), &tFontInfo, &pFont)))
		return E_FAIL;
	TCHAR szBuf[64] = L"";
	swprintf_s(szBuf, L"%d / %d", m_iCurBullet, m_iMaxBullet);
	RECT tempRect = {0,0,g_WINCX,g_WINCY};

	_matrix TempMat;

	D3DXMatrixTranslation(&TempMat, 1128.f, 690.f, 0.f);
	
	m_pShaderCom->Begin_Shader();

	m_pShaderCom->Begin_Pass(0);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Pass();

	m_pShaderCom->End_Shader();

	pSprite->SetTransform(&TempMat);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	pFont->DrawText(
		pSprite, /* 스프라이트 Com 객체*/
		szBuf, /* 출력할 문자열 */
		-1, /* 문자열 길이 */
		&tempRect, /* 사각형 영역 */
		0, /* 사각형 영역 안에 그릴 때의 옵션 */
		D3DCOLOR_ARGB(255, 255, 255, 255) /* 출력 색상 */);
	pSprite->End();

	Safe_Release(pFont);
	Safe_Release(pSprite);

	return S_OK;
}

HRESULT CBulletResiduum::Add_Component()
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

HRESULT CBulletResiduum::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom)
		return E_FAIL;
	
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

	return S_OK;
}

CBulletResiduum * CBulletResiduum::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBulletResiduum* pInstance = new CBulletResiduum(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CBulletResiduum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBulletResiduum::Clone_GameObject(void * pArg)
{
	CBulletResiduum* pInstance = new CBulletResiduum(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CBulletResiduum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBulletResiduum::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

	CGameObject::Free();
}
