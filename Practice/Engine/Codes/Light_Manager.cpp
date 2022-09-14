#include "..\Headers\Light_Manager.h"
#include "..\Headers\Light.h"
#include "..\Headers\Shader.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

D3DLIGHT9 CLight_Manager::Get_LightDesc(_uint iIndex) const
{
	D3DLIGHT9		LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

	if (m_Lights.size() <= iIndex)
		return LightDesc;

	return m_Lights[iIndex]->Get_LightDesc();	
}

HRESULT CLight_Manager::Reserve_Light_Manager(_uint iNumLight)
{
	m_Lights.reserve(iNumLight);

	return NOERROR;
}

HRESULT CLight_Manager::Update_Light_Manager(_double TimeDelta)
{

	for (auto& iter = m_Lights.begin(); iter != m_Lights.end(); iter++)
	{
		(*iter)->Update_Light(TimeDelta);
		if ((*iter)->GetLifeTime() <= 0.0)
		{
			Safe_Release(*iter);
			iter = m_Lights.erase(iter);

			if (iter == m_Lights.end())
				break;
		}
	}
	return S_OK;
}

HRESULT CLight_Manager::Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 * pLightDesc, _double LifeTime)
{
	CLight*	pLight = CLight::Create(pGraphic_Device, pLightDesc, LifeTime);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);

	return NOERROR;
}

HRESULT CLight_Manager::Render_Light(CShader* pShader)
{


	for (auto& pLight : m_Lights)
	{
		if (nullptr != pLight)
			pLight->Render_Light(pShader);
	}
	
	return NOERROR;
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);
	m_Lights.clear();
}
