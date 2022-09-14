#pragma once
#include "Base.h"

BEGIN(Engine)

class CSound : public CBase
{
	DECLARE_SINGLETON(CSound)

public:
	enum CHANNEL_ID { BGM, PLAYER, MONSTER, EFFECT, MAX_CHANNEL };

private:
	explicit CSound();
	virtual ~CSound() = default;

public:
	HRESULT ReadySound();
	HRESULT UpdateSound();
	HRESULT PlaySound(const wstring& wstrSoundKey, CHANNEL_ID eID);
	HRESULT PlayBGM(const wstring& wstrSoundKey);
	HRESULT StopSound(CHANNEL_ID eID);
	HRESULT StopAll();
	HRESULT SetVolume(CHANNEL_ID eID, float fVol);

private:
	void LoadSoundFile();

private:
	// FMOD_SOUND: 사운드 파일에 대한 정보를 갖는 구조체.
	map<wstring, FMOD_SOUND*>	m_MapSound;

	// FMOD_CHANNEL: 사운드를 재생하고 관리하는 역할. ex) 볼륨 조절 등.
	FMOD_CHANNEL* m_pChannelArr[MAX_CHANNEL];

	// FMOD_SYSTEM: FMOD_SOUND과 FMOD_CHANNEL을 총괄 관리하는 객체.
	FMOD_SYSTEM* m_pSystem;
public:
	static CSound* Create();
	virtual void Free();
};

END