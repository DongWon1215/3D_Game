#pragma once

// 1. ����Ÿ�ٵ��� �����Ͽ� �����Ѵ�.
// 2. ������ Ÿ�ٵ��� ��Ƽ� ������´�.(MRT)
// 3. ��Ƽ����Ÿ�ٿ� ��ϵǾ��ִ� Ÿ�ٵ��� ��ġ�� ���Ѵ�.
// 4. ��Ƽ����Ÿ�ٿ� ��ϵǾ��ִ� Ÿ�ٵ��� ȸ���Ѵ�.

#include "Target.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	explicit CTarget_Manager();
	virtual ~CTarget_Manager() = default;
public:
	HRESULT SetUp_OnShader(CShader* pShader, D3DXHANDLE ConstantName, const _tchar* pTargetTag);

public:
	HRESULT Add_Render_Target(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTargetTag, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	HRESULT Begin_MRT(const _tchar* pMRTTag);
	HRESULT End_MRT(const _tchar* pMRTTag);
	
#ifdef _DEBUG
public:
	HRESULT Ready_Debug_Buffer(const _tchar* pTargetTag, _float fStartX, _float fStartY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer(const _tchar* pMRTTag);
#endif // _DEBUG
private:
	map<const _tchar*, CTarget*>				m_Targets;
	typedef map<const _tchar*, CTarget*>		TARGETS;
private:
	map<const _tchar*, list<CTarget*>>			m_MRT;
	typedef map<const _tchar*, list<CTarget*>>	MRT;
private:
	CTarget* Find_RenderTarget(const _tchar* pTargetTag);
	list<CTarget*>* Find_MRT(const _tchar* pMRTTag);
public:
	virtual void Free();

};

END