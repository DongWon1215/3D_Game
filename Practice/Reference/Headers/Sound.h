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
	// FMOD_SOUND: ���� ���Ͽ� ���� ������ ���� ����ü.
	map<wstring, FMOD_SOUND*>	m_MapSound;

	// FMOD_CHANNEL: ���带 ����ϰ� �����ϴ� ����. ex) ���� ���� ��.
	FMOD_CHANNEL* m_pChannelArr[MAX_CHANNEL];

	// FMOD_SYSTEM: FMOD_SOUND�� FMOD_CHANNEL�� �Ѱ� �����ϴ� ��ü.
	FMOD_SYSTEM* m_pSystem;
public:
	static CSound* Create();
	virtual void Free();
};

END