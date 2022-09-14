#pragma once

#include "Base.h"
#include "Shader.h"
#include "Frustum.h"
#include "Texture.h"
#include "Collider.h"
#include "Renderer.h"
#include "Transform.h"
#include "Navigation.h"
#include "Mesh_Static.h"
#include "Mesh_Dynamic.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"

// 컴포넌트 원형들을 모아서 관리하니깐.

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)

private:
	explicit CComponent_Manager();
	virtual ~CComponent_Manager() = default;
public:
	HRESULT Reserve_Component_Manager(_uint iNumScene);
	HRESULT Add_Prototype(_uint iSceneID, const _tchar* pPrototypeTag, CComponent* pComponent);
	CComponent* Clone_Component(_uint iSceneID, const _tchar* pPrototypeTag, void* pArg);
	HRESULT Clear_Scene(_uint iSceneID);
private:
	unordered_map<const _tchar*, CComponent*>*			m_pPrototypes = nullptr;
	typedef unordered_map<const _tchar*, CComponent*>	PROTOTYPES;
private:
	_uint		m_iNumScene;
private:
	CComponent* Find_Prototype(_uint iSceneID, const _tchar* pPrototypeTag);
public:
	virtual void Free();
};

END