#include "stdafx.h"
#include "..\Public\Level_Gacha.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Free.h"
#include "Camera_Ex.h"
#include "Sensei.h"
#include "Student.h"


CLevel_Gacha::CLevel_Gacha(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Gacha::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Formation_BackGround"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Camera(TEXT("Layer_Formation_Camera"))))
	//	return E_FAIL;

	/*if (FAILED(Ready_Layer_Student(TEXT("Layer_Formation_Student"))))
		return E_FAIL;*/





	return S_OK;
}

void CLevel_Gacha::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (KEY(ENTER, AWAY))
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CSensei* pSensei = GET_SENSEI;


		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOBBY))))
			return;

		Safe_Release(pGameInstance);
	}


}

void CLevel_Gacha::Late_Tick(_float TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CSensei* pSensei = GET_SENSEI;

	

}

HRESULT CLevel_Gacha::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("gacha. "));

	return S_OK;
}


HRESULT CLevel_Gacha::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	/*ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(10.f, 5.f, 10.f, 0.f);
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	return E_FAIL;*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Gacha::Ready_Layer_BackGround(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Terrain */
	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_FORMATION, pLayerTag, TEXT("Prototype_GameObject_Formation_Terrain"))))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHA, pLayerTag, TEXT("Prototype_GameObject_RecruitButton"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHA, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Gacha::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Camera_Free */
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(0.f, 0.3f, 5.5f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.3f, 0.f, 1.f);
	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fFovy = XMConvertToRadians(23.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FORMATION, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}



HRESULT CLevel_Gacha::Ready_Layer_Student(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CSensei* pSensei = CSensei::Get_Instance();

	/* For.Player */
	for (int i = 0; i < pSensei->Get_StudentNum(); ++i)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_FORMATION, pLayerTag, pSensei->Get_StudentIndex(i)->Get_StudentInfo().pName)))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}


CLevel_Gacha * CLevel_Gacha::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Gacha*		pInstance = new CLevel_Gacha(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Gacha");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Gacha::Free()
{
	__super::Free();

}
