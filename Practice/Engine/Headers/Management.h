#pragma once

// 1. ������ ����ϱ����� �ʱ�ȭ�۾��� �����Ѵ�.
// 2. ������ ����� ������ ��, �����ϱ����� ����� ���ा����.
// 3. Ŭ���̾�Ʈ���� ������ Ư�� ��ɵ��� �̿��ϰ����Ҷ����� ������ ������ â���� �ȴ�.
// 4. ���� ���ӿ� �������� ���� �ּҸ� �����Ѵ�.(== ���Ŵ����� ��Ȱ)

#include "Device.h"
#include "Input.h"

#include "Light_Manager.h"
#include "Timer_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Target_Manager.h"
#include "Key.h"
#include "Sound.h"

#include "PipeLine.h"

BEGIN(Engine)

class CScene;
class ENGINE_DLL CManagement final : public CBase
{
	DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement();
	virtual ~CManagement() = default;
public:
	const CComponent* Get_Component(_uint iSceneID, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
public:
	// 
	HRESULT Ready_Engine(_uint iNumScene);
	HRESULT Clear_Scene(_uint iSceneID);

public: // For.Graphic_Device
	// �׷��� ����̽��� �����ؼ� ������ ���ڿ� �־��ش�
	HRESULT Ready_Device(HWND hWnd, CDevice::MODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device = nullptr);

public: // For.Input
	// ��ǲ ����̽��� ������ �ش�.
	HRESULT Ready_Input(HINSTANCE hInst, HWND hWnd);
	// ��ǲ ����̽��� ��� �����ϵ��� �ʱ�ȭ ���ش�.
	HRESULT Set_Input_State();
	// ��ǲ ����̽��� Ű������ ���¸� �޾ƿ´�
	_byte Get_DIKState(_ubyte DIKey);
	// ��ǲ ����̽��� ���콺�� ��ư Ŭ�� ������ Ȯ���Ѵ�
	_byte Get_DIMState(CInput::MOUSEBUTTON eDIKey);
	// ��ǲ ����̽��� ���콺�� �������� �����Ѵ�
	_long Get_DIMMove(CInput::MOUSEMOVE eDIMove);

public: // For.Scene
	// ������ ���� ���� ������ ���Ѵ�.
	HRESULT Ready_Current_Scene(CScene* pCurrentScene);
	// ���� ���� ������Ʈ �Լ��� ȣ���Ѵ�
	_int Update_Current_Scene(_double TimeDelta);
	// ���� ���� ���� �Լ��� ȣ���Ѵ�
	HRESULT Render_Current_Scene();

public: // For.Object_Manager
	// ������Ʈ�� ������Ÿ���� �����Ѵ�
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pGameObject);
	// ������ ���̾�� ������Ÿ���� ��������Ѵ�
	HRESULT Add_GameObjectToLayer(const _tchar* pPrototypeTag, _uint iSceneIndex, const _tchar* pLayerTag, void* pArg = nullptr);

	// ������Ʈ �Ŵ����� ������Ʈ ��Ų��.
	HRESULT Update_ObjectManager(_double DeltaTime);

	// ������ �� ���ο� ����ִ� ������ ���̾� �� ������Ʈ�� ���´�
	CGameObject* Get_GameObject(const _tchar* pPrototypeTag, _uint iSceneIndex, const _tchar* pLayerTag, _uint iIndex);

	// ������ ���̾��� ������Ʈ ����Ʈ�� ���´�
	vector<CGameObject*> Get_ObjectList(_uint iSceneID, const _tchar * pLayerTag);

public: // For.Component_Manager
	//������Ʈ�� ������Ÿ���� ������ ���� �����Ѵ�
	HRESULT Add_Prototype(_uint iSceneID, const _tchar* pPrototypeTag, CComponent* pComponent);
	//������ ���� ����� ������Ÿ���� ��������Ѵ�
	CComponent* Clone_Component(_uint iSceneID, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: // For.PipeLine
	// ��ȯ����� ���´�
	_matrix Get_Transform(D3DTRANSFORMSTATETYPE eTransformType);

public: // For.Timer
	// Ÿ�̸Ӹ� �߰��Ѵ�
	HRESULT Add_Timer(const _tchar* pTimerTag);
	// Ÿ�̸Ӹ� ����Ѵ�.
	_double Compute_TimeDelta(const _tchar* pTimerTag);

public: // For.Light
	// ����Ʈ �Ŵ����� ����Ʈ ���� �̸� �Ҵ��Ѵ�
	HRESULT Reserve_Light_Manager(_uint iNumLights);	
	// �Ҵ�� ������ ����Ʈ ��ü�� �����Ѵ�.
	HRESULT Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9* pLightDesc, _double LifeTime = 2.0);
	// ����Ʈ �Ŵ��� �����̳��� ������ �ε��� ��ȣ�� �ش��ϴ� ����Ʈ ��ü�� ���´�
	D3DLIGHT9 Get_LightDesc(_uint iIndex);

public: // For.KeyState
	bool KeyPressing(DWORD dwKey);
	bool KeyDown(DWORD dwKey);
	bool KeyUp(DWORD dwKey);

public: // For.Target
	HRESULT SetRenderTarget_OnShader(CShader* pShader, D3DXHANDLE ConstantName, const _tchar* pTargetTag);

public:	// For. Sound
	HRESULT PlaySound(const wstring& wstrSoundKey, CSound::CHANNEL_ID eID);
	HRESULT PlayBGM(const wstring& wstrSoundKey);
	HRESULT StopSound(CSound::CHANNEL_ID eID);
	HRESULT StopAll();
	HRESULT SetVolume(CSound::CHANNEL_ID eID, float fVol);


public:
	// management�� �Ҵ�� ������ ��ü�� �Ҵ����� �ϰ� ���۷��� ī���͸� ���ҽ�Ų��.
	static HRESULT Release_Engine();

private:
	CDevice*				m_pDevice = nullptr;
	CInput*					m_pInput = nullptr;
	CScene*					m_pCurrentScene = nullptr;
	CObject_Manager*		m_pObject_Manager = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
	CPipeLine*				m_pPipeLine = nullptr;
	CTimer_Manager*			m_pTimer_Manager = nullptr;
	CLight_Manager*			m_pLight_Manager = nullptr;
	CTarget_Manager*		m_pTarget_Manager = nullptr;
	CKey*					m_pKeySTate = nullptr;
	CSound*					m_pSound = nullptr;
public:
	virtual void Free();
};

END