#include "..\Headers\Scene.h"

CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	Safe_AddRef(m_pDevice);
}

HRESULT CScene::Ready_Scene()
{
	return NOERROR;
}

_int CScene::Update_Scene(_double TimeDelta)
{
	return _int();
}

_int CScene::LateUpdate_Scene(_double TimeDelta)
{
	return _int();
}

HRESULT CScene::Render_Scene()
{
	return NOERROR;
}

void CScene::Free()
{
	Safe_Release(m_pDevice);
}
