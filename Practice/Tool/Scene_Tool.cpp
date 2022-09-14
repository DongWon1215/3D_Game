#include "stdafx.h"
#include "Scene_Tool.h"
#include "Management.h"

CScene_Tool::CScene_Tool(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{

}

HRESULT CScene_Tool::Ready_Scene()
{
	return NOERROR;
}

_int CScene_Tool::Update_Scene(_double TimeDelta)
{
	return _int();
}

_int CScene_Tool::LateUpdate_Scene(_double TimeDelta)
{
	
	return _int();
}

HRESULT CScene_Tool::Render_Scene()
{
	return NOERROR;
}

CScene_Tool * CScene_Tool::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Tool* pInstance = new CScene_Tool(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed while Creating CScene_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Tool::Free()
{

	CScene::Free();
}
