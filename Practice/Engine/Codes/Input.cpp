#include "..\Headers\Input.h"

IMPLEMENT_SINGLETON(CInput)

CInput::CInput()
	: m_pSDK(nullptr)
	, m_pKeyBoard(nullptr)
	, m_pMouse(nullptr)
{
	
}


HRESULT CInput::Ready_Input(HINSTANCE hInst, HWND hWnd)
{
	// 내 키보드와 마우스의 상태를 조사 및 셋팅 할 수 있는 객체를 생성한다(m_pSDK)
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pSDK, nullptr)))
		return E_FAIL;
	
	if (FAILED(m_pSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND/* | DISCL_NOWINKEY*/)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->Acquire()))
		return E_FAIL;



	if (FAILED(m_pSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND/* | DISCL_NOWINKEY*/)))
		return E_FAIL;

	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CInput::Set_Input_State()
{
	if (nullptr == m_pKeyBoard ||
		nullptr == m_pMouse)
		return E_FAIL;

	if (FAILED(m_pKeyBoard->GetDeviceState(256, m_byKeyBoardState)))
		return E_FAIL;

	if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState)))
		return E_FAIL;


	return NOERROR;
}

void CInput::Free()
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pSDK);
}
