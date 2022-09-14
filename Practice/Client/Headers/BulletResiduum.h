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

class CBulletResiduum final : public CGameObject
{
private:
	explicit CBulletResiduum(LPDIRECT3DDEVICE9 pDevice);
	explicit CBulletResiduum(const CBulletResiduum& rhs);
	virtual ~CBulletResiduum() = default;

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
	_float m_fSizeX, m_fSizeY, m_fStartX, m_fStartY;
	_uint m_iCurBullet = 0, m_iMaxBullet = 0;

	WEAPON m_ePlayerWeapon;
	

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CBulletResiduum* Create(LPDIRECT3DDEVICE9 pDevice); // ¿øÇü°´Ã¼
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free();
};
