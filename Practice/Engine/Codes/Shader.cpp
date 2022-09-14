#include "..\Headers\Shader.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_pErrMessage(rhs.m_pErrMessage)
{
	Safe_AddRef(m_pEffect);
	Safe_AddRef(m_pErrMessage);
}

HRESULT CShader::Ready_Component_Prototype(const _tchar * pShaderFilePath)
{	
	// ÀÌÆåÆ® °´Ã¼¸¦ »ý¼ºÇÑ´Ù.
	if (FAILED(D3DXCreateEffectFromFile(Get_Graphic_Device(), pShaderFilePath, nullptr, nullptr, 0, nullptr, &m_pEffect, &m_pErrMessage)))
	{
		MessageBoxA(0, (char*)m_pErrMessage->GetBufferPointer(), nullptr, MB_OK);
		return E_FAIL;
	}
		
	
	return NOERROR;
}

HRESULT CShader::Ready_Component(void * pArg)
{
	return NOERROR;
}

HRESULT CShader::Begin_Shader()
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->Begin(&m_iNumPasses, 0);

	return NOERROR;
}

HRESULT CShader::Begin_Pass(_uint iPassIndex)
{
	if (nullptr == m_pEffect || 
		m_iNumPasses <= iPassIndex)
		return E_FAIL;

	m_pEffect->BeginPass(iPassIndex);

	return NOERROR;
}

HRESULT CShader::End_Pass()
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->EndPass();
	
	return NOERROR;
}

HRESULT CShader::End_Shader()
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->End();

	return NOERROR;
}

HRESULT CShader::Commit_Change()
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	return m_pEffect->CommitChanges();	
}

HRESULT CShader::Set_Value(D3DXHANDLE hHandle, void * pData, _uint iSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	return m_pEffect->SetValue(hHandle, pData, iSize);	
}

HRESULT CShader::Set_Texture(D3DXHANDLE hHandle, LPDIRECT3DBASETEXTURE9 pTexture)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	return m_pEffect->SetTexture(hHandle, pTexture);
}

HRESULT CShader::Set_Bool(D3DXHANDLE hHandle, bool pBool)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	return m_pEffect->SetBool(hHandle, pBool);
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pShaderFilePath)
{
	CShader* pInstance = new CShader(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype(pShaderFilePath)))
	{
		MSG_BOX("Failed while Creating CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CShader::Clone_Component(void * pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed while Creating CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CShader::Free()
{
	Safe_Release(m_pErrMessage);
	Safe_Release(m_pEffect);

	CComponent::Free();
}
