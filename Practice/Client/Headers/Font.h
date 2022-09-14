#pragma once
#include "Client_Extend.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

class CPlayer;
class CFont final : public CGameObject
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 pDevice);
	explicit CFont(const CFont& rhs);
	virtual ~CFont() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();// ¿øÇü°´Ã¼
	virtual HRESULT Ready_GameObject(void* pArg);// º¹»ç°´Ã¼(½ÇÁ¦»ç¿ëµÇ´Â°´Ã¼)
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	void SetNumber(_uint iNum) { m_iNum = iNum; }

private:
	CTransform*				m_pTransformCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT DevideNumber();

private:
	int m_iNum = 0;

	list<int> m_lstNumber;

public:
	static CFont* Create(LPDIRECT3DDEVICE9 pDevice); // ¿øÇü°´Ã¼
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free();
};