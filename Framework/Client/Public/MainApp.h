#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)
//class CGameInstance;
class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT Initialize();	
	void Tick(float fTimeDelta);
	HRESULT Render();

private:
	CGameInstance*			m_pGameInstance = nullptr;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	CRenderer*				m_pRenderer = nullptr;

private:
	/* 그리기 상태를 셋팅하낟. */
	HRESULT SetUp_RenderState();
	HRESULT SetUp_SamplerState();

	HRESULT Open_Level(LEVEL eLevelID);
	HRESULT Ready_Prototype_Component();

public:
	

public:
	static CMainApp* Create();
	virtual void Free();

};

END