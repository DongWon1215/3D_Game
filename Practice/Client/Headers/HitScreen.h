#pragma once
#include "GameObject.h"
#include "Client_Extend.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

class CHitScreen final : public CGameObject
{
private:
	explicit CHitScreen(LPDIRECT3DDEVICE9 pDevice);
	explicit CHitScreen(const CHitScreen& rhs);
	virtual ~CHitScreen() = default ;

public:
	virtual HRESULT Ready_GameObject_Prototype();// ¿øÇü°´Ã¼
	virtual HRESULT Ready_GameObject(void* pArg);// º¹»ç°´Ã¼(½ÇÁ¦»ç¿ëµÇ´Â°´Ã¼)
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CTransform*				m_pTransformCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	_vec3 m_vPosition;
	_float m_fLifeTime;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CHitScreen* Create(LPDIRECT3DDEVICE9 pDevice); // ¿øÇü°´Ã¼
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free();
};

