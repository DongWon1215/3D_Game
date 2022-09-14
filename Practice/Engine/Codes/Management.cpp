#include "../Headers/Management.h"
#include "..\Headers\Scene.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
{

}

const CComponent * CManagement::Get_Component(_uint iSceneID, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iSceneID, pLayerTag, pComponentTag, iIndex);
}

// 멤버변수 초기화
HRESULT CManagement::Ready_Engine(_uint iNumScene)
{
	// For.Graphic_Device
	// 장치 초기화
	m_pDevice = CDevice::GetInstance();
	if (nullptr == m_pDevice)
		return E_FAIL;

	Safe_AddRef(m_pDevice);
	//input 장치 추가
	m_pInput = CInput::GetInstance();
	if (nullptr == m_pInput)
		return E_FAIL;

	Safe_AddRef(m_pInput);


	// For.Object_Manager
	// 오브젝트들을 매니저로 통합해서 관리
	m_pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Safe_AddRef(m_pObject_Manager);

	if (FAILED(m_pObject_Manager->Reserve_Object_Manager(iNumScene)))
		return E_FAIL;

	// For.Component_Manager
	m_pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	Safe_AddRef(m_pComponent_Manager);

	if (FAILED(m_pComponent_Manager->Reserve_Component_Manager(iNumScene)))
		return E_FAIL;

	// For.PipeLine
	m_pPipeLine = CPipeLine::GetInstance();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	Safe_AddRef(m_pPipeLine);


	// For.Timer_Manager
	m_pTimer_Manager = CTimer_Manager::GetInstance();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	Safe_AddRef(m_pTimer_Manager);

	// For.Light_Manager
	m_pLight_Manager = CLight_Manager::GetInstance();
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	Safe_AddRef(m_pLight_Manager);

	m_pKeySTate = CKey::GetInstance();
	if (nullptr == m_pKeySTate)
		return E_FAIL;
	
	m_pSound = CSound::GetInstance();
	if (nullptr == m_pSound)
		return E_FAIL;


	return NOERROR;
}

// 특정 씬 초기화
HRESULT CManagement::Clear_Scene(_uint iSceneID)
{
	if (nullptr == m_pComponent_Manager ||
		nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Clear_Scene(iSceneID)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Clear_Scene(iSceneID)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CManagement::Ready_Device(HWND hWnd, CDevice::MODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppDevice)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(m_pDevice->Ready_Device(hWnd, eMode, iWinCX, iWinCY, ppDevice)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CManagement::Ready_Input(HINSTANCE hInst, HWND hWnd)
{
	if (nullptr == m_pInput)
		return E_FAIL;

	return m_pInput->Ready_Input(hInst, hWnd);	
}

HRESULT CManagement::Set_Input_State()
{
	if (nullptr == m_pInput)
		return E_FAIL;

	return m_pInput->Set_Input_State();
}

_byte CManagement::Get_DIKState(_ubyte DIKey)
{
	if (nullptr == m_pInput)
		return 0;

	return _byte(m_pInput->Get_DIKState(DIKey));
}

_byte CManagement::Get_DIMState(CInput::MOUSEBUTTON eDIKey)
{
	if (nullptr == m_pInput)
		return 0;

	return _byte(m_pInput->Get_DIMState(eDIKey));
}

_long CManagement::Get_DIMMove(CInput::MOUSEMOVE eDIMove)
{
	if (nullptr == m_pInput)
		return 0;

	return _byte(m_pInput->Get_DIMMove(eDIMove));
}

HRESULT CManagement::Ready_Current_Scene(CScene * pCurrentScene)
{
	if (nullptr == pCurrentScene)
		return E_FAIL;

	if(0 != Safe_Release(m_pCurrentScene))
	{
		MSG_BOX("Failed while Release CurrentScene");
		return E_FAIL;
	}

	m_pCurrentScene = pCurrentScene;

	Safe_AddRef(m_pCurrentScene);

	return NOERROR;
}

_int CManagement::Update_Current_Scene(_double TimeDelta)
{
	if (nullptr == m_pCurrentScene || 
		nullptr == m_pObject_Manager)
		return -1;

	_int iUpdateEvent = 0;

	m_pObject_Manager->Update_GameObject_Manager(TimeDelta);
	m_pObject_Manager->LateUpdate_GameObject_Manager(TimeDelta);
	
	m_pLight_Manager->Update_Light_Manager(TimeDelta);

	m_pKeySTate->Componenet_Update(TimeDelta);

	iUpdateEvent = m_pCurrentScene->Update_Scene(TimeDelta);
	if (iUpdateEvent & 0x80000000)
		return iUpdateEvent;

	iUpdateEvent = m_pCurrentScene->LateUpdate_Scene(TimeDelta);
	if (iUpdateEvent & 0x80000000)
		return iUpdateEvent;
	
	return _int();
}

HRESULT CManagement::Render_Current_Scene()
{
	if (nullptr == m_pCurrentScene)
		return E_FAIL;

	return m_pCurrentScene->Render_Scene();
}

HRESULT CManagement::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pGameObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pGameObject);
}

HRESULT CManagement::Add_GameObjectToLayer(const _tchar * pPrototypeTag, _uint iSceneIndex, const _tchar * pLayerTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObjectToLayer(pPrototypeTag, iSceneIndex, pLayerTag, pArg);	
}

HRESULT CManagement::Update_ObjectManager(_double DeltaTime)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pObject_Manager->Update_GameObject_Manager(DeltaTime);
	m_pObject_Manager->LateUpdate_GameObject_Manager(DeltaTime);

	return S_OK;
}

CGameObject* CManagement::Get_GameObject(const _tchar * pPrototypeTag, _uint iSceneIndex, const _tchar * pLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return (CGameObject*)m_pObject_Manager->Get_GameObject(iSceneIndex,pLayerTag,iIndex);
}

vector<CGameObject*> CManagement::Get_ObjectList(_uint iSceneID, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return vector<CGameObject*>();

	return (vector<CGameObject*>)m_pObject_Manager->Get_ObjectList(iSceneID, pLayerTag);
}

HRESULT CManagement::Add_Prototype(_uint iSceneID, const _tchar * pPrototypeTag, CComponent * pComponent)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iSceneID, pPrototypeTag, pComponent);
}

CComponent * CManagement::Clone_Component(_uint iSceneID, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iSceneID, pPrototypeTag, pArg);
}

_matrix CManagement::Get_Transform(D3DTRANSFORMSTATETYPE eTransformType)
{
	_matrix		matTmp;

	D3DXMatrixIdentity(&matTmp);

	if (nullptr == m_pPipeLine)
		return matTmp;

	return m_pPipeLine->Get_Transform(eTransformType);
}

HRESULT CManagement::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(pTimerTag);
}

_double CManagement::Compute_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Compute_TimeDelta(pTimerTag);
}

HRESULT CManagement::Reserve_Light_Manager(_uint iNumLights)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Reserve_Light_Manager(iNumLights);
}

HRESULT CManagement::Add_Light(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9 * pLightDesc, _double LifeTime)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pDevice, pLightDesc, LifeTime);
}

D3DLIGHT9 CManagement::Get_LightDesc(_uint iIndex)
{
	D3DLIGHT9		LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

	if (nullptr == m_pLight_Manager)
		return LightDesc;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

bool CManagement::KeyPressing(DWORD dwKey)
{
	return m_pKeySTate->KeyPressing(dwKey);
}

bool CManagement::KeyDown(DWORD dwKey)
{
	return m_pKeySTate->KeyDown(dwKey);
}

bool CManagement::KeyUp(DWORD dwKey)
{
	return m_pKeySTate->KeyUp(dwKey);
}



HRESULT CManagement::SetRenderTarget_OnShader(CShader * pShader, D3DXHANDLE ConstantName, const _tchar * pTargetTag)
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	return m_pTarget_Manager->SetUp_OnShader(pShader, ConstantName, pTargetTag);
}

HRESULT CManagement::PlaySound(const wstring & wstrSoundKey, CSound::CHANNEL_ID eID)
{
	return m_pSound->PlaySoundW(wstrSoundKey, eID);
}

HRESULT CManagement::PlayBGM(const wstring & wstrSoundKey)
{
	return m_pSound->PlayBGM(wstrSoundKey);
}

HRESULT CManagement::StopSound(CSound::CHANNEL_ID eID)
{
	return m_pSound->StopSound(eID);
}

HRESULT CManagement::StopAll()
{
	return m_pSound->StopAll();
}

HRESULT CManagement::SetVolume(CSound::CHANNEL_ID eID, float fVol)
{
	return m_pSound->SetVolume(eID,fVol);
}

HRESULT CManagement::Release_Engine()
{
	_ulong		dwRefCnt = 0;

	if (dwRefCnt = CManagement::GetInstance()->DestroyInstance())
		MSG_BOX("Failed while Release CManagement");

	if (dwRefCnt = CObject_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed while Release CObject_Manager");

	if (dwRefCnt = CComponent_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed while Release CComponent_Manager");

	if (dwRefCnt = CPipeLine::GetInstance()->DestroyInstance())
		MSG_BOX("Failed while Release CPipeLine");	

	if (dwRefCnt = CLight_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed while Release CLight_Manager");

	if (dwRefCnt = CTarget_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed while Release CInput");

	if (dwRefCnt = CDevice::GetInstance()->DestroyInstance())
		MSG_BOX("Failed while Release CDevice");

	if (dwRefCnt = CTimer_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed while Release CTimer");

	if (dwRefCnt = CInput::GetInstance()->DestroyInstance())
		MSG_BOX("Failed while Release CInput");

	if (dwRefCnt = CSound::GetInstance()->DestroyInstance())
		MSG_BOX("Failed while Release CSound");

	return NOERROR;
}

void CManagement::Free()
{

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pCurrentScene);
	Safe_Release(m_pKeySTate);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pSound);

	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pInput);
	Safe_Release(m_pDevice);
}
