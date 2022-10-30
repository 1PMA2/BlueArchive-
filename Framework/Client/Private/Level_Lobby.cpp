#include "stdafx.h"
#include "..\Public\Level_Lobby.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Sensei.h"
#include "Student.h"


CLevel_Lobby::CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Lobby::Initialize()
{

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Lobby(TEXT("Layer_Lobby_BG"))))
		return E_FAIL;

	CSensei* pSensei = GET_SENSEI;

	pSensei->Release_FormationStudents();

	pSensei->Set_End(false);
	CGameInstance::Get_Instance()->StopSound(BGM);
	CGameInstance::Get_Instance()->PlayBGM(L"ClearMorning_Short", 0.5f);
	return S_OK;
}

void CLevel_Lobby::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	Open_Formation();
	
	Open_Gacha();

}

void CLevel_Lobby::Late_Tick(_float TimeDelta)
{

}

HRESULT CLevel_Lobby::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Lobby"));

	return S_OK;
}

void CLevel_Lobby::Open_Formation()
{
	CSensei* pSensei = GET_SENSEI;

	pSensei->Reset_FormationAry();

	if (0 < pSensei->Get_FormationInfoSize())
	{
		for (_uint i = 0; i < pSensei->Get_FormationInfoSize(); ++i)
		{
			pSensei->Set_FormationAry(pSensei->Get_FormationInfo(i).eFormation, true);
		}
	}
	
	

	if (pSensei->Get_OpenFormationLevel())
	{
		pSensei->Set_PreLevel(LEVEL_LOBBY);

		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_FORMATION))))
			return;

		Safe_Release(pGameInstance);

		pSensei->Set_OpenFormationLevel(false);
	}
}

void CLevel_Lobby::Open_Gacha()
{
	CSensei* pSensei = GET_SENSEI;

	if (pSensei->Get_OpenGacha())
	{
		pSensei->Set_PreLevel(LEVEL_LOBBY);

		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GACHA))))
			return;

		Safe_Release(pGameInstance);

		pSensei->Set_OpenGacha(false);
	}
}

HRESULT CLevel_Lobby::Ready_Layer_Lobby(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_int iImgNum = 0;

	/* For.BackGround */
	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_BG"), &iImgNum)))
	//	return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Workspace"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_GachaButton"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_WorkButton"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}


CLevel_Lobby * CLevel_Lobby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Lobby*		pInstance = new CLevel_Lobby(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Lobby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Lobby::Free()
{
	__super::Free();



}