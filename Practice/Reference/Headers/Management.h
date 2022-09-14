#pragma once

// 1. 엔진을 사용하기위한 초기화작업을 수행한다.
// 2. 엔진의 사용이 끝났을 때, 정리하기위한 기능을 수행ㅎ나다.
// 3. 클라이언트에서 엔진의 특정 기능들을 이용하고자할때마다 일종의 소통의 창구가 된다.
// 4. 현재 게임에 보여지는 씬의 주소를 보관한다.(== 씬매니져의 역활)

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
	// 그래픽 디바이스를 생성해서 마지막 인자에 넣어준다
	HRESULT Ready_Device(HWND hWnd, CDevice::MODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device = nullptr);

public: // For.Input
	// 인풋 디바이스를 생성해 준다.
	HRESULT Ready_Input(HINSTANCE hInst, HWND hWnd);
	// 인풋 디바이스를 사용 가능하도록 초기화 해준다.
	HRESULT Set_Input_State();
	// 인풋 디바이스중 키보드의 상태를 받아온다
	_byte Get_DIKState(_ubyte DIKey);
	// 인풋 디바이스중 마우스의 버튼 클릭 유무를 확인한다
	_byte Get_DIMState(CInput::MOUSEBUTTON eDIKey);
	// 인풋 디바이스중 마우스의 움직임을 감지한다
	_long Get_DIMMove(CInput::MOUSEMOVE eDIMove);

public: // For.Scene
	// 임의의 씬을 현재 씬으로 정한다.
	HRESULT Ready_Current_Scene(CScene* pCurrentScene);
	// 현재 씬의 업데이트 함수를 호출한다
	_int Update_Current_Scene(_double TimeDelta);
	// 현재 씬의 랜더 함수를 호출한다
	HRESULT Render_Current_Scene();

public: // For.Object_Manager
	// 오브젝트의 프로토타입을 선언한다
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pGameObject);
	// 임의의 레이어에서 프로토타입을 복사생성한다
	HRESULT Add_GameObjectToLayer(const _tchar* pPrototypeTag, _uint iSceneIndex, const _tchar* pLayerTag, void* pArg = nullptr);

	// 오브젝트 매니저를 업데이트 시킨다.
	HRESULT Update_ObjectManager(_double DeltaTime);

	// 임의의 씬 내부에 들어있는 임의의 레이어 속 오브젝트를 얻어온다
	CGameObject* Get_GameObject(const _tchar* pPrototypeTag, _uint iSceneIndex, const _tchar* pLayerTag, _uint iIndex);

	// 임의의 레이어의 오브젝트 리스트를 얻어온다
	vector<CGameObject*> Get_ObjectList(_uint iSceneID, const _tchar * pLayerTag);

public: // For.Component_Manager
	//컴포넌트의 프로토타입을 임의의 씬에 선언한다
	HRESULT Add_Prototype(_uint iSceneID, const _tchar* pPrototypeTag, CComponent* pComponent);
	//임의의 씬에 선언된 프로토타입을 복사생성한다
	CComponent* Clone_Component(_uint iSceneID, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: // For.PipeLine
	// 변환행렬을 얻어온다
	_matrix Get_Transform(D3DTRANSFORMSTATETYPE eTransformType);

public: // For.Timer
	// 타이머를 추가한다
	HRESULT Add_Timer(const _tchar* pTimerTag);
	// 타이머를 계산한다.
	_double Compute_TimeDelta(const _tchar* pTimerTag);

public: // For.Light
	// 라이트 매니저의 라이트 수를 미리 할당한다
	HRESULT Reserve_Light_Manager(_uint iNumLights);	
	// 할당된 공간에 라이트 객체를 삽입한다.
	HRESULT Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9* pLightDesc, _double LifeTime = 2.0);
	// 라이트 매니저 컨테이너의 임의의 인덱스 번호에 해당하는 라이트 객체를 얻어온다
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
	// management에 할당된 포인터 객체를 할당해제 하고 레퍼런스 카운터를 감소시킨다.
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