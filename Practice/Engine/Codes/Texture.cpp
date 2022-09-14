#include "..\Headers\Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_Textures(rhs.m_Textures)
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);

}

IDirect3DBaseTexture9 * CTexture::Get_Texture(_uint iIndex)
{
	if (m_Textures.size() <= iIndex)
		return nullptr;
	
	return m_Textures[iIndex];
}

HRESULT CTexture::Ready_Component_Prototype(TEXTURETYPE eType, const _tchar * pFilePath, _uint iNumTexture)
{
	_tchar		szFilePath[MAX_PATH] = L"";	

	m_Textures.reserve(iNumTexture);

	for (size_t i = 0; i < iNumTexture; ++i)
	{
		IDirect3DBaseTexture9*		pTexture = nullptr;

		wsprintf(szFilePath, pFilePath, i);

		HRESULT hr = 0;

		switch (eType)
		{
		case CTexture::TYPE_CUBE:
			hr = D3DXCreateCubeTextureFromFile(Get_Graphic_Device(), szFilePath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
			break;
		case CTexture::TYPE_GENERAL:
			hr = D3DXCreateTextureFromFile(Get_Graphic_Device(), szFilePath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;
		}

		if (FAILED(hr))
			return E_FAIL;		

		m_Textures.push_back(pTexture);
	}	

	return NOERROR;
}

HRESULT CTexture::Ready_Component(void * pArg)
{
	return NOERROR;
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURETYPE eType, const _tchar * pFilePath, _uint iNumTexture)
{
	CTexture* pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype(eType, pFilePath, iNumTexture)))
	{
		MSG_BOX("Failed while Creating CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
	}

CComponent * CTexture::Clone_Component(void * pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed while Creating CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

	m_Textures.clear();

	CComponent::Free();
}
