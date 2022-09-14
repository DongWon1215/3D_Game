#pragma once
#include "GameObject.h"
#include "Client_Extend.h"

BEGIN(Engine)
class CShader;
class CFrustum;
class CTexture;
class CRenderer;
class CTransform;
class CNavigation;
class CVIBuffer_Terrain;
END


BEGIN(Client)

class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();
private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pFilterCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Terrain*		m_pVIBufferCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CFrustum*				m_pFrustumCom = nullptr;

private:
	_vec3 m_vBrushPos;
	_bool m_bIsShow = false;

private:
	HRESULT Add_Component();
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice); // 원본을 생성한다.
	virtual CGameObject* Clone_GameObject(void* pArg); // 복제본을 생성한다.
	virtual void Free();
};

END