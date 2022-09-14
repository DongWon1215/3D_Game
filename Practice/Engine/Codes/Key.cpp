#include "..\Headers\Key.h"

IMPLEMENT_SINGLETON(CKey)

CKey::CKey()
{
}

void CKey::Componenet_Update(_double deltaTime)
{
	m_dwKey = 0;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_RETURN;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_dwKey |= KEY_SHIFT;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_W;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_A;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_S;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_D;
	if (GetAsyncKeyState('R') & 0x8000)
		m_dwKey |= KEY_R;
	if (GetAsyncKeyState('1') & 0x8000)
		m_dwKey |= KEY_1;
	if (GetAsyncKeyState('2') & 0x8000)
		m_dwKey |= KEY_2;
	if (GetAsyncKeyState('3') & 0x8000)
		m_dwKey |= KEY_3;
	if (GetAsyncKeyState(VK_F1) & 0x8000)
		m_dwKey |= KEY_F1;
	if (GetAsyncKeyState(VK_F2) & 0x8000)
		m_dwKey |= KEY_F2;
	if (GetAsyncKeyState(VK_F3) & 0x8000)
		m_dwKey |= KEY_F3;
	if (GetAsyncKeyState(VK_F4) & 0x8000)
		m_dwKey |= KEY_F4;
}

bool CKey::KeyPressing(DWORD dwKey)
{
	if (m_dwKey & dwKey)
		return true;

	return false;
}

bool CKey::KeyDown(DWORD dwKey)
{
	// 이전에 누른적 없고 현재 눌렀을 때 TRUE
	if (!(m_dwKeyDown & dwKey) && (m_dwKey & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}

	// 이전에 누른적 있고 현재 누르지 않으면 FALSE
	if ((m_dwKeyDown & dwKey) && !(m_dwKey & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

bool CKey::KeyUp(DWORD dwKey)
{
	// 이전에 누른적 있고 현재 누르지 않았을 때 TRUE
	if ((m_dwKeyUp & dwKey) && !(m_dwKey & dwKey))
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}

	// 이전에 누른적 없고 현재 눌렸을 때 FALSE
	if (!(m_dwKeyUp & dwKey) && (m_dwKey & dwKey))
	{
		m_dwKeyUp |= dwKey;
		return false;
	}

	return false;
}


void CKey::Free()
{
}
