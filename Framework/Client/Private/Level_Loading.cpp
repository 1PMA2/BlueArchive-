#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_Lobby.h"
#include "Level_Formation.h"
#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Sensei.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (LEVEL_LOGO != m_eNextLevel)
	{
		if (FAILED(Ready_Layer_LoadingImage(TEXT("Layer_LoadingImage"))))
			return E_FAIL;
	}

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	if (true == m_pLoader->is_Finished())
	{
		CSensei*  pSensei = GET_SENSEI;

		CLevel*			pLevel = nullptr;

			switch (m_eNextLevel)
			{
			case LEVEL_LOGO:
				pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_LOBBY:
				if ((LEVEL_LOGO == pSensei->Get_CurrentLevel()) || KEY(LBUTTON, AWAY))
				{
					pLevel = CLevel_Lobby::Create(m_pDevice, m_pContext);
				}
				break;
			case LEVEL_FORMATION:
				pLevel = CLevel_Formation::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pLevel)
				return;

			CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pLevel)))
				return;

			Safe_Release(pGameInstance);
	}
}

void CLevel_Loading::Late_Tick(_float TimeDelta)
{
}

HRESULT CLevel_Loading::Render()
{

	if (FAILED(__super::Render()))
			return E_FAIL;

	

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());	

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_LoadingImage(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_LoadingImage"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLevel_Loading*		pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}

