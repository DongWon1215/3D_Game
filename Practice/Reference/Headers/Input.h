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
	// Ű������ ���¸� Ȯ���Ͽ� �Ű������� Ű���� ����
	_byte Get_DIKState(_ubyte DIKey) const {
		return m_byKeyBoardState[DIKey]; }

	// ���콺�� ��ư Ŭ�� ������ Ȯ��
	_byte Get_DIMState(MOUSEBUTTON eDIKey) const {
		return m_MouseState.rgbButtons[eDIKey];  }

	//���콺�� �����Ӱ� ���� �������� �����ؼ� ����
	_long Get_DIMMove(MOUSEMOVE eDIMove) const {
		return *((_long*)&m_MouseState + eDIMove); 	}

public:
	//Input Device�� ����ϱ� ���� ��� ������ �ʱ�ȭ
	HRESULT Ready_Input(HINSTANCE hInst, HWND hWnd);

	HRESULT Set_Input_State(); // �� Ű����� ���콺�� ���¸� �����Ѵ�.

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