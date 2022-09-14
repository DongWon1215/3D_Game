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

class CProgress final : public CGameObject
{
private:
	explicit CProgress(LPDIRECT3DDEVICE9 pDevice);
	explicit CProgress(const CProgress& rhs);
	virtual ~CProgress() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();// ������ü
	virtual HRESULT Ready_GameObject(void* pArg);// ���簴ü(�������Ǵ°�ü)
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	void Set_Progress(_uint piProgress) { m_iProgress = piProgress; }

private:
	CTransform*				m_pTransformCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
private:
	_float m_fSizeX, m_fSizeY, m_fStartX, m_fStartY;
	_uint m_iProgress;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CProgress* Create(LPDIRECT3DDEVICE9 pGraphic_Device); // ������ü
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free();
};

