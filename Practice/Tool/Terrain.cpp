#include "stdafx.h"
#include "Terrain.h"
#include "Management.h"
#include "MainFrm.h"
#include "ToolView.h"


#include "Cell.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
	m_vVertex.clear();
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
	,m_vVertex(rhs.m_vVertex)
{
}

CTerrain::CTerrain(const CTerrain & rhs, _uint XVertex, _uint ZVertex, _float fInterval)
	: CGameObject(rhs)
	, m_vVertex(rhs.m_vVertex)
{
	m_pVIBufferCom->Ready_Component_Prototype(XVertex, ZVertex, fInterval);
}

HRESULT CTerrain::Ready_GameObject_Prototype() // 원본객체의 초기화.
{
	/*m_pDevice->SetSamplerState(D3DSAMP_MINFILTER)*/
	return NOERROR;
}

HRESULT CTerrain::Ready_GameObject(void* pArg) // 복제된객체의 추가 초기화.
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	
	return NOERROR;
}

_int CTerrain::Update_GameObject(_double TimeDelta)
{
	_vec3 vBrushPos;

	m_dwKey = 0;

	if (m_pVIBufferCom->Picking(m_pTransformCom, &vBrushPos))
	{
		LPDIRECT3DVERTEXBUFFER9 pVB = m_pVIBufferCom->GetVertexBuffer();
		VTXNORTEX* pVertices = nullptr;
		D3DXVec3TransformCoord(&vBrushPos, &vBrushPos, &m_pTransformCom->Get_WorldMatrix());

		if (m_bIsTerrain)
		m_pShaderCom->Set_Value("g_vBrushPosition", &vBrushPos, sizeof(_vec4));

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			m_dwKey |= KEY_LBUTTON;
			if (m_bIsTerrain)
			{
				pVB->Lock(0, 0, (void**)&pVertices, 0);
				for (_uint i = 0; i < m_pVIBufferCom->GetVerticesNum(); i++)
				{
					if (CalculateDistance(pVertices[i].vPosition, vBrushPos))
					{
						if (m_bIsUp)
							pVertices[i].vPosition.y += 0.2f;
						else
							pVertices[i].vPosition.y -= 0.2f;
					}
				}
			}
		}
		if (m_bIsKeyDown() && !m_bIsTerrain)
		{
			_vec3 Vertex = _vec3(roundf(vBrushPos.x * 100 / 100), 0, roundf(vBrushPos.z * 100 / 100));

			m_vVertex.push_back(Vertex);

			SetCells();
		}
		pVB->Unlock();
	}
	return _int();
}

_int CTerrain::LateUpdate_GameObject(_double TimeDelta)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());

	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CTerrain::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	_matrix		matTransform;
	D3DXMatrixIdentity(&matTransform);
	_matrix			matProj;


	m_pShaderCom->Set_Value("g_matWorld", &matTransform, sizeof(_matrix));

	m_pShaderCom->Set_Value("g_matView", &pManagement->Get_Transform(D3DTS_VIEW), sizeof(_matrix));

	m_pShaderCom->Set_Value("g_matProj", &pManagement->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));


	// For.LightDesc
	D3DLIGHT9 LightDesc = pManagement->Get_LightDesc(0);

	
	if (LightDesc.Type == D3DLIGHT_POINT)
	{
		m_iPassIndex = 2;
		m_pShaderCom->Set_Value("g_vLightPosition", &_vec4(LightDesc.Position, 1.f), sizeof(_vec4));
		m_pShaderCom->Set_Value("g_fRange", &LightDesc.Range, sizeof(_float));
	}

	m_pShaderCom->Set_Value("g_vLightDir", &_vec4(LightDesc.Direction, 0.f), sizeof(_vec4));
	m_pShaderCom->Set_Value("g_vLightDiffuse", &LightDesc.Diffuse, sizeof(_vec4));
	m_pShaderCom->Set_Value("g_vLightAmbient", &LightDesc.Ambient, sizeof(_vec4));
	m_pShaderCom->Set_Value("g_vLightSpecular", &LightDesc.Specular, sizeof(_vec4));

	_matrix ViewMatrix = pManagement->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pShaderCom->Set_Value("g_vCamPosition", &ViewMatrix.m[3][0], sizeof(_vec4));
	
	m_pShaderCom->Set_Texture("g_SourTexture", m_pTextureCom->Get_Texture(0));
	m_pShaderCom->Set_Texture("g_DestTexture", m_pTextureCom->Get_Texture(1));
	m_pShaderCom->Set_Texture("g_FilterTexture", m_pFilterCom->Get_Texture(0));
	m_pShaderCom->Set_Texture("g_BrushTexture", m_pBrushCom->Get_Texture(0));

	m_pShaderCom->Begin_Shader();
	m_pShaderCom->Begin_Pass(m_iPassIndex);

	m_pVIBufferCom->Render_VIBuffer();

#ifdef  _DEBUG
	RanderCell();
#endif

	m_pShaderCom->End_Pass();
	m_pShaderCom->End_Shader();


	Safe_Release(pManagement);

	return NOERROR;
}

bool CTerrain::CalculateDistance(_vec3  VertexPosition, _vec3 TargetVec)
{
	float distance = sqrt(pow(TargetVec.x - VertexPosition.x,2) + pow(TargetVec.z - VertexPosition.z,2));
	if (distance < m_fRange)
	{
		return true;
	}

	return false;
}

void CTerrain::CalculVector()
{
	_vec3 vAB, vBC, vNormal;

	vAB = m_vVertex[1] - m_vVertex[0];
	vBC = m_vVertex[2] - m_vVertex[1];

	D3DXVec3Normalize(&vAB, &vAB);
	D3DXVec3Normalize(&vBC, &vBC);

	D3DXVec3Cross(&vNormal, &vAB, &vBC);

	if (vNormal.y < 0)
	{
		_vec3 temp = m_vVertex[1];
		m_vVertex[1] = m_vVertex[2];
		m_vVertex[2] = temp;
	}

}

void CTerrain::EraseCell()
{
	m_Cells.pop_back();
	for (int i = 0; i < 3; i++)
	{
		m_vRealVertex.pop_back();
	}
	m_iCellCount--;
}

void CTerrain::SetCells()
{
	if (m_vVertex.size() >= 3)
	{
		CalculVector();
		CCell* pCell = CCell::Create(Get_Graphic_Device(), m_vVertex[0], m_vVertex[1],	m_vVertex[2], m_iCellCount);
		if (nullptr == pCell)
			return;

		m_iCellCount++;
		
		m_Cells.push_back(pCell);

		m_vRealVertex;

		for (int i = 0; i < 3; i++)
		{
			m_vRealVertex.push_back(m_vVertex[0]);
			m_vVertex.erase(m_vVertex.begin());
		}
	}
}

vector<_vec3> CTerrain::GetVertexVector()
{
	return m_vRealVertex;
}

vector<CCell*> CTerrain::GetCellVector()
{
	return m_Cells;
}

_vec3 CTerrain::GetVertexPos()
{
	if (!m_bIsTerrain)
	{
		_vec3 vVertexPos;

		m_pVIBufferCom->Picking(m_pTransformCom, &vVertexPos);


		return vVertexPos;
	}

	return _vec3(0.f, 0.f, 0.f);
}

HRESULT CTerrain::Add_Component()
{
	if(FAILED(CGameObject::Add_Component(0, L"Component_Shader_Terrain", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if(FAILED(CGameObject::Add_Component(0, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if(FAILED(CGameObject::Add_Component(0, L"Component_Texture_Terrain", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if(FAILED(CGameObject::Add_Component(0, L"Component_Texture_Filter", L"Com_Filter", (CComponent**)&m_pFilterCom)))
		return E_FAIL;

	if(FAILED(CGameObject::Add_Component(0, L"Component_Texture_Brush", L"Com_Brush", (CComponent**)&m_pBrushCom)))
		return E_FAIL;

	if(FAILED(CGameObject::Add_Component(0, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	
	if(FAILED(CGameObject::Add_Component(0, L"Component_VIBuffer_Terrain", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return NOERROR;
}

bool CTerrain::m_bIsKeyDown()
{
	if ((m_dwKeyDown & KEY_LBUTTON) && !(m_dwKey & KEY_LBUTTON))
	{
		m_dwKeyDown ^= KEY_LBUTTON;
		return true;
	}

	// 이전에 누른적 없고 현재 눌렸을 때 FALSE
	if (!(m_dwKeyDown & KEY_LBUTTON) && (m_dwKey & KEY_LBUTTON))
	{
		m_dwKeyDown |= KEY_LBUTTON;
		return false;
	}

	return false;
}

#ifdef  _DEBUG
HRESULT CTerrain::RanderCell()
{
	for (int i = 0; i < m_Cells.size(); i++)
		m_Cells[i]->Render_Cell();

	return S_OK;
}
#endif //  _DEBUG

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTerrain* pInstance = new CTerrain(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed while Creating CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone_GameObject(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed while Creating CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	m_vVertex.clear();
	m_vRealVertex.clear();

	for (auto& iter = m_Cells.begin(); iter != m_Cells.end(); iter++)
	{
		Safe_Release(*iter);
	}
	m_Cells.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBrushCom);
	Safe_Release(m_pFilterCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
