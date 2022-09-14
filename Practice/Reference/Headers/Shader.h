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
	// 쉐이더 파일의 랜더 패스중 사용할 랜더패스를 선택한다 
	HRESULT Begin_Pass(_uint iPassIndex);
	HRESULT End_Pass();
	HRESULT End_Shader();
	// 쉐이더를 랜더 후 지우고 다시 그림
	HRESULT Commit_Change();
public:
	HRESULT Set_Value(D3DXHANDLE hHandle, void* pData, _uint iSize);	
	HRESULT Set_Texture(D3DXHANDLE hHandle, LPDIRECT3DBASETEXTURE9 pTexture);
	HRESULT Set_Bool(D3DXHANDLE hHandle, bool pBool);
private:
	LPD3DXEFFECT			m_pEffect = nullptr; // 셰이더 파일을 컴파일한 셰이더를 이용하기위한 인터페이스를 가지고 있는 객체(셰이더를 대표하는 객체)
	LPD3DXBUFFER			m_pErrMessage = nullptr;
	_uint					m_iNumPasses = 0;
public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END