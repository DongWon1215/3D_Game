#include "..\Headers\Light.h"
#include "..\Headers\VIBuffer_ViewportRect.h"
#include "..\Headers\Shader.h"

CLight::CLight(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pDevice(pGraphic_Device)
{
	Safe_AddRef(m_pDevice);
}

HRESULT CLight::Ready_Light(const D3DLIGHT9 * pLightDesc, _double LifeTime)
{
	m_LightDesc = *pLightDesc;

	D3DVIEWPORT9		ViewPort;
	m_pDevice->GetViewport(&ViewPort);

	m_pBuffer = CVIBuffer_ViewportRect::Create(m_pDevice, 0.f - 0.5f, 0.f - 0.5f, ViewPort.Width, ViewPort.Height);
	if (nullptr == m_pBuffer)
		return E_FAIL;

	m_LifeTime = LifeTime;

	return NOERROR;
}

HRESULT CLight::Update_Light(_double TimeDelta)
{
	if (m_LightDesc.Type != D3DLIGHT_DIRECTIONAL)
		m_LifeTime -= 0.1f;

	return S_OK;
}

HRESULT CLight::Render_Light(CShader * pShader)
{
	if (nullptr == m_pBuffer)
		return E_FAIL;

	_uint iPassIndex = 0;

	if (D3DLIGHT_DIRECTIONAL == m_LightDesc.Type)
	{
		iPassIndex = 0;
		pShader->Set_Value("g_vLightDir", &_vec4(m_LightDesc.Direction, 0.f), sizeof(_vec4));
	}

	if (D3DLIGHT_POINT == m_LightDesc.Type)
	{
		iPassIndex = 1;
		pShader->Set_Value("g_vLightPos", &_vec4(m_LightDesc.Position, 1.f), sizeof(_vec4));
		pShader->Set_Value("g_fRange", &m_LightDesc.Range, sizeof(_float));
	}
	pShader->Set_Value("g_vLightDiffuse", &m_LightDesc.Diffuse, sizeof(_vec4));
	pShader->Set_Value("g_vLightAmbient", &m_LightDesc.Ambient, sizeof(_vec4));
	pShader->Set_Value("g_vLightSpecular", &m_LightDesc.Specular, sizeof(_vec4));

	pShader->Commit_Change();

	pShader->Begin_Pass(iPassIndex);

	m_pBuffer->Render_VIBuffer();

	pShader->End_Pass();
	return S_OK;
}

CLight * CLight::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 * pLightInfo, _double LifeTime)
{
	CLight* pInstance = new CLight(pGraphic_Device);

	if (FAILED(pInstance->Ready_Light(pLightInfo, LifeTime)))
	{
		MSG_BOX("Failed while Creating CLight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pBuffer);
	Safe_Release(m_pDevice);
}
