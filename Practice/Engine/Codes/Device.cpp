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

	// ��ġ �ʱ�ȭ.

	// 1. IDirect3D9 ��ü ����
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == m_pSDK)
		return E_FAIL;

	// 2. ��ġ ����
	// 2-1. HAL�� ���� ��ġ ������ ����.
	D3DCAPS9	d3dcaps;
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	// GetDeviceCaps: �׷���ī�带 �����ؼ� ������ D3DCAPS9 ����ü�� ��Ƴ���.
	// HAL(Hardware Abstraction Layer, �ϵ���� �߻� ����)
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps)))
		return E_FAIL;

	// 2-2. ���� �׷��� ��ġ�� ���ؽ� ���μ����� �����ϴ°� ����
	// *���ؽ� ���μ���: ���� ��ȯ + ���� ó��
	DWORD vp = 0;

	if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	vp |= D3DCREATE_MULTITHREADED;

	// 3. IDirect3DDevice9 ��ü ����
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	// ������ ȯ�� ����.
	d3dpp.BackBufferWidth = iWinCX;	// �ĸ���� ���� ������
	d3dpp.BackBufferHeight = iWinCY; // �ĸ���� ���� ������
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // �ĸ���� �ȼ� ����
	d3dpp.BackBufferCount = 1; // �ĸ���� ����

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // ���� ü�� ���
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode; // TRUE â���, FALSE ��üȭ����
	d3dpp.EnableAutoDepthStencil = TRUE; // ���̹��ۿ� ���ٽǹ��۸� ��ġ�� �ڵ� ����.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // ���� ����� �ֻ����� �����.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // ��� �ÿ� �Ѵ�.

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
	// Com��ü ����	
	if (m_pDevice->Release())
		MessageBox(0, L"m_pDevice Release Failed", L"System Error", MB_OK);
	if(m_pSDK->Release())
		MessageBox(0, L"m_pSDK Release Failed", L"System Error", MB_OK);
}
