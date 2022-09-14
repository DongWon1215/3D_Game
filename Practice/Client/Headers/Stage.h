#pragma once

#include "Client_Extend.h"
#include "Scene.h"
#include "BossLoading.h"
BEGIN(Client)


class CStage final : public CScene
{
public:
	typedef struct tagObject
	{
		D3DXMATRIX WorldMatrix;
		const _tchar* FileName;			//메쉬 이름, 검색용, 맵의 키값
		const _tchar* ProtoTypeName;	//프로토 타입 컴포넌트 이름
		const _tchar* ObjectName;		//오브젝트 파일 이름

	}OBJECT;

private:
	explicit CStage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStage() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(_double TimeDelta);
	virtual _int LateUpdate_Scene(_double TimeDelta);
	virtual HRESULT Render_Scene();

public:
	_uint Get_RemainingNum() { return m_iRemaining; }

private:
	HRESULT Ready_Light_Desc();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

private:
	void SetMonsterNum();

private:
	CBossLoading*		m_pLoading = nullptr;

private:
	_uint				m_iWave = 0;
	_uint				m_iRemaining = 0;

	_float				m_fWaitingTime = 0.f;

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free();
};
END
