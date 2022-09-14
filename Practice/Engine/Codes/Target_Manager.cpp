#include "..\Headers\Target_Manager.h"
#include "..\Headers\Shader.h"

IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{

}

HRESULT CTarget_Manager::SetUp_OnShader(CShader * pShader, D3DXHANDLE ConstantName, const _tchar * pTargetTag)
{
	CTarget*	pTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pTarget)
		return E_FAIL;

	return pTarget->SetUp_OnShader(pShader, ConstantName);
}

HRESULT CTarget_Manager::Add_Render_Target(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pTargetTag, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor)
{
	if (nullptr != Find_RenderTarget(pTargetTag))
		return E_FAIL;

	CTarget*		pRenderTarget = CTarget::Create(pGraphic_Device, iWidth, iHeight, eFormat, ClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_Targets.insert(TARGETS::value_type(pTargetTag, pRenderTarget));

	return NOERROR;
}

HRESULT CTarget_Manager::Add_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag)
{
	CTarget*	pTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pTarget)
		return E_FAIL;

	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CTarget*>		MRTList;

		MRTList.push_back(pTarget);

		m_MRT.insert(MRT::value_type(pMRTTag, MRTList));
	}
	else
		pMRTList->push_back(pTarget);

	Safe_AddRef(pTarget);

	return NOERROR;
}

HRESULT CTarget_Manager::Begin_MRT(const _tchar * pMRTTag)
{
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pTarget : *pMRTList)
	{		
		if (FAILED(pTarget->Clear_Target()))
			return E_FAIL;
	}

	_uint iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{		
		if (FAILED(pTarget->SetUp_OnGraphicDev(iIndex++)))
			return E_FAIL;
	}	

	return NOERROR;
}

HRESULT CTarget_Manager::End_MRT(const _tchar * pMRTTag)
{
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	_uint iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{
		pTarget->Release_OnGraphicDev(iIndex++);
	}

	return NOERROR;
}

#ifdef _DEBUG
HRESULT CTarget_Manager::Ready_Debug_Buffer(const _tchar * pTargetTag, _float fStartX, _float fStartY, _float fSizeX, _float fSizeY)
{
	CTarget*	pTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pTarget)
		return E_FAIL;

	return pTarget->Ready_Debug_Buffer(fStartX, fStartY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render_Debug_Buffer(const _tchar * pMRTTag)
{
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pTarget : *pMRTList)
	{
		pTarget->Render_Debug_Buffer();
	}

	return NOERROR;
}
#endif // _DEBUG

CTarget * CTarget_Manager::Find_RenderTarget(const _tchar * pTargetTag)
{
	auto	iter = find_if(m_Targets.begin(), m_Targets.end(), CTag_Finder(pTargetTag));
	if (iter == m_Targets.end())
		return nullptr;

	return iter->second;
}

list<CTarget*>* CTarget_Manager::Find_MRT(const _tchar * pMRTTag)
{
	auto	iter = find_if(m_MRT.begin(), m_MRT.end(), CTag_Finder(pMRTTag));
	if (iter == m_MRT.end())
		return nullptr;

	return &iter->second;	
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_MRT)
	{
		for (auto& pTarget : Pair.second)
			Safe_Release(pTarget);
		Pair.second.clear();
	}


	m_MRT.clear();

	for (auto& Pair : m_Targets)
		Safe_Release(Pair.second);

	m_Targets.clear();	
}
