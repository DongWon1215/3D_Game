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

class CBoxPuzzle final : public CGameObject
{
	typedef struct TagBox
	{
		RECT rc;

		_bool bIsTresure;

		_vec3 vecPos;
	}BOX;

private:
	explicit CBoxPuzzle(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoxPuzzle(const CBoxPuzzle& rhs);
	virtual ~CBoxPuzzle() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();// ¿øÇü°´Ã¼
	virtual HRESULT Ready_GameObject(void* pArg);// º¹»ç°´Ã¼(½ÇÁ¦»ç¿ëµÇ´Â°´Ã¼)
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	void ShowButtonToggle() { m_bIsShow = !m_bIsShow; }

private:
	CTransform*				m_pTransformCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	_bool					m_bIsShow;
	_bool					m_bIsClear;

	_uint					m_iLife = 3;
	_uint					m_iStartX, m_iStartY;


	BOX						m_RcBox[100];

private:
	HRESULT FindTresure();
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT ReSet();

private:

	void Reward();

public:
	static CBoxPuzzle* Create(LPDIRECT3DDEVICE9 pDevice); // ¿øÇü°´Ã¼
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free();
};