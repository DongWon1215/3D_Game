#include "..\Headers\Layer.h"
#include "..\Headers\GameObject.h"

CLayer::CLayer()
{
}

const CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	if (m_ObjectList.size() <= iIndex)
		return nullptr;

	auto	iter = m_ObjectList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;	

	return (*iter)->Get_Component(pComponentTag);
}

const CGameObject* CLayer::Get_GameObject(_uint iIndex)
{
	auto& iter = m_ObjectList;

	return iter.at(iIndex);
}

const vector<CGameObject*> CLayer::Get_ObjectList() const
{
	return m_ObjectList;
}

HRESULT CLayer::Ready_Layer()
{
	return NOERROR;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_ObjectList.push_back(pGameObject);

	return NOERROR;
}

_int CLayer::Update_Layer(_double TimeDelta)
{
	for (auto& pGameObject : m_ObjectList)
	{
		if (nullptr != pGameObject)
			pGameObject->Update_GameObject(TimeDelta);
	}

	return _int();
}

_int CLayer::LateUpdate_Layer(_double TimeDelta)
{
	for (auto& iter = m_ObjectList.begin(); iter != m_ObjectList.end(); iter++)
	{
		if (iter != m_ObjectList.end())
			(*iter)->LateUpdate_GameObject(TimeDelta);

		if ((*iter)->GetIsDead())
		{
			Safe_Release(*iter);
			iter = m_ObjectList.erase(iter);
			
			if (iter == m_ObjectList.end())
			{
				if (m_ObjectList.size() <= 0)
				{
					m_ObjectList.clear();
					m_ObjectList.shrink_to_fit();
				}
				break;
			}
		}
	}

	return _int();
}

CLayer * CLayer::Create()
{
	CLayer* pInstance = new CLayer();

	if (FAILED(pInstance->Ready_Layer()))
	{
		MSG_BOX("Failed while Creating CLayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_ObjectList)
	{
		Safe_Release(pGameObject);
	}

	m_ObjectList.clear();
}
