#include "..\Headers\Target.h"
#include "..\Headers\Shader.h"

CTarget::CTarget(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pDevice(pGraphic_Device)
{
	Safe_AddRef(m_pDevice);
}

HRESULT CTarget::Ready_Target(_uint iWidth, _uint iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor)
{
	// 렌더타겟용으로 활용한 텍스쳐를 생성하였다.
	if (FAILED(D3DXCreateTexture(m_pDevice, iWidth, iHeight, 1, D3DUSAGE_RENDERTARGET, Format, D3DPOOL_DEFAULT, &m_pTarget_Texture)))
		return E_FAIL;

	// 이렇게 생성한 텍스쳐로부터 렌더타겟(IDirect3DSurface9)을 얻어온다(변환한다)
	if (FAILED(m_pTarget_Texture->GetSurfaceLevel(0, &m_pTarget_Surface)))
		return E_FAIL;

	m_ClearColor = ClearColor;

	return NOERROR;
}

HRESULT CTarget::SetUp_OnGraphicDev(_uint iIndex)
{
	if (nullptr == m_pDevice ||
		nullptr == m_pTarget_Surface)
		return E_FAIL;

	m_pDevice->GetRenderTarget(iIndex, &m_pOld_Surface);		

	if (FAILED(m_pDevice->SetRenderTarget(iIndex, m_pTarget_Surface)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTarget::SetUp_OnShader(CShader * pShader, D3DXHANDLE pConstantName)
{
	if (nullptr == pShader || 
		nullptr == m_pTarget_Texture)
		return E_FAIL;

	pShader->Set_Texture(pConstantName, m_pTarget_Texture);

	return NOERROR;
}

HRESULT CTarget::Release_OnGraphicDev(_uint iIndex)
{
	if (nullptr == m_pDevice ||
		nullptr == m_pTarget_Surface)
		return E_FAIL;	

	m_pDevice->SetRenderTarget(iIndex, m_pOld_Surface);

	Safe_Release(m_pOld_Surface);

	return NOERROR;
}

HRESULT CTarget::Clear_Target()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	SetUp_OnGraphicDev(0);

	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0);

	Release_OnGraphicDev(0);	

	return NOERROR;
}

#ifdef _DEBUG
HRESULT CTarget::Ready_Debug_Buffer(_float fStartX, _float fStartY, _float fSizeX, _float fSizeY)
{
	m_pDebugBuffer = CVIBuffer_ViewportRect::Create(m_pDevice, fStartX, fStartY, fSizeX, fSizeY);

	if (nullptr == m_pDebugBuffer)
		return E_FAIL;

	return NOERROR;
}
HRESULT CTarget::Render_Debug_Buffer()
{
	if (nullptr == m_pDebugBuffer)
		return E_FAIL;

	m_pDevice->SetTexture(0, m_pTarget_Texture);
	m_pDebugBuffer->Render_VIBuffer();

	return NOERROR;
}
#endif // _DEBUG

CTarget * CTarget::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWidth, _uint iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor)
{
	CTarget* pInstance = new CTarget(pGraphic_Device);

	if (FAILED(pInstance->Ready_Target(iWidth, iHeight, Format, ClearColor)))
	{
		MSG_BOX("Failed while Creating CTarget");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTarget::Free()
{

#ifdef _DEBUG
	Safe_Release(m_pDebugBuffer);
#endif // _DEBUG

	Safe_Release(m_pTarget_Texture);
	Safe_Release(m_pOld_Surface);
	Safe_Release(m_pTarget_Surface);

	Safe_Release(m_pDevice);
}
