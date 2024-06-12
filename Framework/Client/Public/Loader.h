#pragma once

/* 다음레벨에 필요한 자원을 로드하낟. */

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	CRITICAL_SECTION Get_CS() const {
		return m_CriticalSection;
	}

	LEVEL Get_NextLevelID() const {
		return m_eNextLevel;
	}

	_bool is_Finished() const {
		return m_isFinished;
	}

	_bool is_TexFinished() const {
		return m_isTexFinished;
	}

	const _tchar* Get_LoadingText() const {
		return m_szLoadingText;
	}

public:
	HRESULT Initialize(LEVEL eNextLevel);
	HRESULT Loading_ForLogoLevel();
	HRESULT Loading_ForLobbyLevel();
	HRESULT Loading_ForGachaLevel();
	HRESULT Loading_ForGachaScene();
	HRESULT Loading_ForGamePlayLevel();
	HRESULT Loading_ForFormationLevel();

private:
	D3D11_TEXTURE2D_DESC		desc;
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;	
	LEVEL						m_eNextLevel = LEVEL_END;

private:
	HANDLE					m_hThread;
	CRITICAL_SECTION		m_CriticalSection;
	_bool					m_isFinished = false;
	_bool					m_isTexFinished = false;
	_tchar					m_szLoadingText[MAX_PATH] = TEXT("");

public:

	
public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel);
	virtual void Free() override;
};

END