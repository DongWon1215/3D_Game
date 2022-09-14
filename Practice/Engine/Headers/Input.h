#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput final : public CBase
{
	DECLARE_SINGLETON(CInput)
public:
	enum MOUSEBUTTON { DIM_LBUTTON, DIM_RBUTTON, DIM_WHEEL};
	enum MOUSEMOVE { DIM_X, DIM_Y, DIM_Z };
private:
	explicit CInput();
	virtual ~CInput() = default;
public:	
	// 키보드의 상태를 확인하여 매개변수의 키값을 리턴
	_byte Get_DIKState(_ubyte DIKey) const {
		return m_byKeyBoardState[DIKey]; }

	// 마우스의 버튼 클릭 유무를 확인
	_byte Get_DIMState(MOUSEBUTTON eDIKey) const {
		return m_MouseState.rgbButtons[eDIKey];  }

	//마우스의 움직임과 휠의 움직임을 감지해서 리턴
	_long Get_DIMMove(MOUSEMOVE eDIMove) const {
		return *((_long*)&m_MouseState + eDIMove); 	}

public:
	//Input Device를 사용하기 위해 멤버 변수를 초기화
	HRESULT Ready_Input(HINSTANCE hInst, HWND hWnd);

	HRESULT Set_Input_State(); // 내 키보드와 마우스의 상태를 저장한다.

private:
	LPDIRECTINPUT8			m_pSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;
private:	
	_byte					m_byKeyBoardState[256];	
	DIMOUSESTATE			m_MouseState;
public:
	virtual void Free();

};

END