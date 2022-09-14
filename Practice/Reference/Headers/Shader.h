#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;
public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pShaderFilePath);
	virtual HRESULT Ready_Component(void* pArg);
public:
	HRESULT Begin_Shader();
	// ���̴� ������ ���� �н��� ����� �����н��� �����Ѵ� 
	HRESULT Begin_Pass(_uint iPassIndex);
	HRESULT End_Pass();
	HRESULT End_Shader();
	// ���̴��� ���� �� ����� �ٽ� �׸�
	HRESULT Commit_Change();
public:
	HRESULT Set_Value(D3DXHANDLE hHandle, void* pData, _uint iSize);	
	HRESULT Set_Texture(D3DXHANDLE hHandle, LPDIRECT3DBASETEXTURE9 pTexture);
	HRESULT Set_Bool(D3DXHANDLE hHandle, bool pBool);
private:
	LPD3DXEFFECT			m_pEffect = nullptr; // ���̴� ������ �������� ���̴��� �̿��ϱ����� �������̽��� ������ �ִ� ��ü(���̴��� ��ǥ�ϴ� ��ü)
	LPD3DXBUFFER			m_pErrMessage = nullptr;
	_uint					m_iNumPasses = 0;
public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END