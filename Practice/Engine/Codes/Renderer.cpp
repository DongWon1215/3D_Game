#include "..\Headers\Renderer.h"
#include "..\Headers\GameObject.h"
#include "..\Headers\Light_Manager.h"
#include "..\Headers\Shader.h"
#include "..\Headers\PipeLine.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pPipeLine);
}


HRESULT CRenderer::Ready_Component_Prototype()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	D3DVIEWPORT9		ViewPort;
	Get_Graphic_Device()->GetViewport(&ViewPort);

	// For.Target_Diffuse	
	if (FAILED(m_pTarget_Manager->Add_Render_Target(Get_Graphic_Device(), L"Target_Diffuse", ViewPort.Width, ViewPort.Height, D3DFMT_A8R8G8B8, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// For.Target_Normal
	if (FAILED(m_pTarget_Manager->Add_Render_Target(Get_Graphic_Device(), L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Depth
	if (FAILED(m_pTarget_Manager->Add_Render_Target(Get_Graphic_Device(), L"Target_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 1.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Shade
	if (FAILED(m_pTarget_Manager->Add_Render_Target(Get_Graphic_Device(), L"Target_Shade", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Specular
	if (FAILED(m_pTarget_Manager->Add_Render_Target(Get_Graphic_Device(), L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;


	// For.MRT_Deferred : 디퍼드로 그려져야할 객체들의 특정한 정보들을 담는 타겟들. 
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Diffuse")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Normal")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Depth")))
		return E_FAIL;

	// For.MRT_LightAcc : 빛연산의 결과정보를 누적하여 저장할 타겟들.
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Shade")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Specular")))
		return E_FAIL;

	m_pShader_LightAcc = CShader::Create(Get_Graphic_Device(), L"../Bin/Shader/Shader_LightAcc.fx");
	if (nullptr == m_pShader_LightAcc)
		return E_FAIL;
	m_pShader_Blend = CShader::Create(Get_Graphic_Device(), L"../Bin/Shader/Shader_Blend.fx");
	if (nullptr == m_pShader_Blend)
		return E_FAIL;

	m_pBuffer = CVIBuffer_ViewportRect::Create(Get_Graphic_Device(), 0.f - 0.5f, 0.f - 0.5f, ViewPort.Width, ViewPort.Height);
	if (nullptr == m_pBuffer)
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Diffuse", 0.f, 0.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Normal", 0.f, 100.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Depth", 0.f, 200.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Shade", 100.f, 0.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Specular", 100.f, 100.f, 100.f, 100.f)))
		return E_FAIL;
#endif // _DEBUG


	return NOERROR;
}

HRESULT CRenderer::Ready_Component(void * pArg)
{


	return NOERROR;
}

HRESULT CRenderer::Add_RenderList(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject ||
		RENDER_END <= eGroup)
		return E_FAIL;

	m_RenderList[eGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return NOERROR;
}

_bool Compare(CGameObject* pSour, CGameObject* pDest)
{
	return pSour->Get_ViewZ() > pDest->Get_ViewZ();
}

HRESULT CRenderer::Render_RenderList()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_NonAlpha()))
		return E_FAIL;
	if (FAILED(Render_Light()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_Alpha()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;

#ifndef _Tool
#ifdef _DEBUG
	//if (m_bIsShow)
	//{
	//	m_pTarget_Manager->Render_Debug_Buffer(L"MRT_Deferred");
	//	m_pTarget_Manager->Render_Debug_Buffer(L"MRT_LightAcc");
	//}
#endif // _DEBUG
#endif

	return NOERROR;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_PRIORITY].clear();

	return NOERROR;
}

HRESULT CRenderer::Render_NonAlpha()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	// 디퓨즈 + 노멀타겟을 장치에 셋팅한다.
	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Deferred")))
		return E_FAIL;


	// 디퓨즈 + 노멀타겟에 객체들의 임의의 정보를 저장해준다.
	for (auto& pGameObject : m_RenderList[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_NONALPHA].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Deferred")))
		return E_FAIL;

	return NOERROR;
}

HRESULT CRenderer::Render_Alpha()
{
	m_RenderList[RENDER_ALPHA].sort(Compare);

	for (auto& pGameObject : m_RenderList[RENDER_ALPHA])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_ALPHA].clear();

	return NOERROR;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_UI].clear();

	return NOERROR;
}

HRESULT CRenderer::Render_Light()
{
	if (nullptr == m_pTarget_Manager ||
		nullptr == m_pShader_LightAcc ||
		nullptr == m_pPipeLine)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_LightAcc")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->SetUp_OnShader(m_pShader_LightAcc, "g_NormalTexture", L"Target_Normal")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(m_pShader_LightAcc, "g_DepthTexture", L"Target_Depth")))
		return E_FAIL;

	_matrix		ViewMatrix = m_pPipeLine->Get_Transform(D3DTS_VIEW);
	_matrix		ProjMatrix = m_pPipeLine->Get_Transform(D3DTS_PROJECTION);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);

	m_pShader_LightAcc->Set_Value("g_matViewInv", &ViewMatrix, sizeof(_matrix));
	m_pShader_LightAcc->Set_Value("g_matProjInv", &ProjMatrix, sizeof(_matrix));

	m_pShader_LightAcc->Set_Value("g_vCamPosition", &ViewMatrix.m[3][0], sizeof(_vec4));

	m_pShader_LightAcc->Begin_Shader();


	// 장치에 셰이드 + 타겟이 셋팅되어있다. 
	m_pLight_Manager->Render_Light(m_pShader_LightAcc);

	m_pShader_LightAcc->End_Shader();

	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_LightAcc")))
		return E_FAIL;

	return NOERROR;
}

HRESULT CRenderer::Render_Blend()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pShader_Blend)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->SetUp_OnShader(m_pShader_Blend, "g_DiffuseTexture", L"Target_Diffuse")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(m_pShader_Blend, "g_ShadeTexture", L"Target_Shade")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(m_pShader_Blend, "g_SpecularTexture", L"Target_Specular")))
		return E_FAIL;

	m_pShader_Blend->Begin_Shader();
	m_pShader_Blend->Begin_Pass(0);

	m_pBuffer->Render_VIBuffer();

	m_pShader_Blend->End_Pass();
	m_pShader_Blend->End_Shader();

	return NOERROR;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer* pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MSG_BOX("Failed while Creating CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CRenderer::Clone_Component(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderList[i])
			Safe_Release(pGameObject);

		m_RenderList[i].clear();
	}

	Safe_Release(m_pPipeLine);
	Safe_Release(m_pShader_LightAcc);
	Safe_Release(m_pShader_Blend);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);

	this;

	CComponent::Free();
}

