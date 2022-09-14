#include "../Headers/Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_pSDK(nullptr)
	, m_pDevice(nullptr)
{

}

HRESULT CDevice::Ready_Device(HWND hWnd, MODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device)
{
	HRESULT hr = 0;

	// 장치 초기화.

	// 1. IDirect3D9 객체 생성
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == m_pSDK)
		return E_FAIL;

	// 2. 장치 조사
	// 2-1. HAL을 통해 장치 정보를 얻어옴.
	D3DCAPS9	d3dcaps;
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	// GetDeviceCaps: 그래픽카드를 조사해서 정보를 D3DCAPS9 구조체에 담아낸다.
	// HAL(Hardware Abstraction Layer, 하드웨어 추상 계층)
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps)))
		return E_FAIL;

	// 2-2. 현재 그래픽 장치가 버텍스 프로세싱을 지원하는가 조사
	// *버텍스 프로세싱: 정점 변환 + 조명 처리
	DWORD vp = 0;

	if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	vp |= D3DCREATE_MULTITHREADED;

	// 3. IDirect3DDevice9 객체 생성
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	// 렌더링 환경 설정.
	d3dpp.BackBufferWidth = iWinCX;	// 후면버퍼 가로 사이즈
	d3dpp.BackBufferHeight = iWinCY; // 후면버퍼 세로 사이즈
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // 후면버퍼 픽셀 포맷
	d3dpp.BackBufferCount = 1; // 후면버퍼 개수

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // 스왑 체인 방식
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode; // TRUE 창모드, FALSE 전체화면모드
	d3dpp.EnableAutoDepthStencil = TRUE; // 깊이버퍼와 스텐실버퍼를 장치가 자동 관리.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // 현재 모니터 주사율에 맞춘다.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // 즉시 시연 한다.

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_pDevice)))
		return E_FAIL;

	if (nullptr != ppGraphic_Device)
	{
		*ppGraphic_Device = m_pDevice;
		Safe_AddRef(m_pDevice);
	}
	
	
	return S_OK;
}

void CDevice::Free()
{
	// Com객체 해제	
	if (m_pDevice->Release())
		MessageBox(0, L"m_pDevice Release Failed", L"System Error", MB_OK);
	if(m_pSDK->Release())
		MessageBox(0, L"m_pSDK Release Failed", L"System Error", MB_OK);
}
