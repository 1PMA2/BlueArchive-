#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CSound_Device final : public CBase
{
	DECLARE_SINGLETON(CSound_Device)

public:
	CSound_Device();
	~CSound_Device();

public:
	HRESULT		Initialize();
	void		Update();
public:
	virtual void Free();

public:
	void		LoadSoundFile();
	void		Play_Sound(const _tchar* strSoundKey, _float fVolume = 1.f);
	void		Play_Sound_Player(const _tchar* strSoundKey, _float fVolume = 1.f);
	void		PlayBGM(const _tchar* strSoundKey, _float fVolume);
	void		StopSound(CHANNELID eType);
	void		StopAll();
	void		WithoutBGM();
	void		SetVolume(_float fVolume);
	void		SetChannelVolume(CHANNELID eID, _float fVolume);

private:
	map <_tchar*, FMOD_SOUND*> m_mapSound;
	FMOD_CHANNEL* m_pChannelArr[CH_END];
	FMOD_SYSTEM* m_pSystem;

	_int m_eCurChannel;
	_int m_iCurPlayerChannel;
	_int m_iCount = 0;
};

END