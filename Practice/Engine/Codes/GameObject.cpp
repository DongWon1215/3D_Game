#include "..\Headers\GameObject.h"
#include "..\Headers\Management.h"
#include "..\Headers\Component.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	Safe_AddRef(m_pDevice);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
{
	Safe_AddRef(m_pDevice);
}

const CComponent * CGameObject::Get_Component(const _tchar * pComponentTag) const
{
	return Find_Component(pComponentTag);	
}

HRESULT CGameObject::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CGameObject::Ready_GameObject(void* pArg)
{
	return NOERROR;
}

_int CGameObject::Update_GameObject(_double TimeDelta)
{
	return _int();
}

_int CGameObject::LateUpdate_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CGameObject::Render_GameObject()
{
	return NOERROR;
}

HRESULT CGameObject::Add_Component(_uint iSceneID, const _tchar * pPrototypeTag, const _tchar * pComponentTag, CComponent ** ppComponent, void* pArg)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CComponent* pComponent = pManagement->Clone_Component(iSceneID, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.insert(COMPONENTS::value_type(pComponentTag, pComponent));

	*ppComponent = pComponent;

	Safe_AddRef(pComponent);

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CGameObject::Compute_ViewZ(_vec3 vWorldPos)
{
	CPipeLine*		pPipeLine = CPipeLine::GetInstance();
	if (nullptr == pPipeLine)
		return E_FAIL;

	Safe_AddRef(pPipeLine);

	_matrix		matView;

	D3DXMatrixInverse(&matView, nullptr, &pPipeLine->Get_Transform(D3DTS_VIEW));

	m_fViewZ = D3DXVec3Length(&(vWorldPos - *(_vec3*)&matView.m[3][0]));

	Safe_Release(pPipeLine);

	return NOERROR;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag) const
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));
	if (iter == m_Components.end())
		return nullptr;
	return iter->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)	
		Safe_Release(Pair.second);

	m_Components.clear();	

	Safe_Release(m_pDevice);
}
