#include "stdafx.h"
#include "..\Public\Gacha_Scene.h"

#include "GameInstance.h"
#include "Loader.h"
#include "Level_Loading.h"
#include "Camera_Free.h"
#include "Camera_Ex.h"
#include "Sensei.h"
#include "Student.h"
#include "Level_Lobby.h"


CGacha_Scene::CGacha_Scene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CGacha_Scene::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Arona(TEXT("Layer_Arona"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Student(TEXT("Layer_Student"))))
		//return E_FAIL;




	return S_OK;
}

void CGacha_Scene::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (KEY(ENTER, AWAY))
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);


		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GACHA))))
			return;


		Safe_Release(pGameInstance);
	}

}

void CGacha_Scene::Late_Tick(_float TimeDelta)
{
	//CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CSensei* pSensei = GET_SENSEI;



}

HRESULT CGacha_Scene::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("gacha Scene. "));

	return S_OK;
}


HRESULT CGacha_Scene::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 0.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CGacha_Scene::Ready_Layer_BackGround(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Terrain */
	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_FORMATION, pLayerTag, TEXT("Prototype_GameObject_Formation_Terrain"))))
	//	return E_FAIL;
	_int iImgNum = 1;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHASCENE, pLayerTag, TEXT("Prototype_GameObject_BackGround"), &iImgNum)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CGacha_Scene::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHASCENE, pLayerTag, TEXT("Prototype_GameObject_AronaCAM"))))
		return E_FAIL;

	/* For.Camera_Free */
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(0.f, 1.f, 5.5f, 1.f);
	CameraDesc.vAt = _float4(0.f, 1.f, 0.f, 1.f);
	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fFovy = XMConvertToRadians(10.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHASCENE, pLayerTag, TEXT("Prototype_GameObject_Camera_Gacha"), &CameraDesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}



HRESULT CGacha_Scene::Ready_Layer_Arona(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Player */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHASCENE, pLayerTag, TEXT("Prototype_GameObject_Arona"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHASCENE, pLayerTag, TEXT("Prototype_GameObject_AronaSack"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CGacha_Scene::Ready_Layer_Student(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CSensei* pSensei = GET_SENSEI;

	_int iImgNum = pSensei->Get_Student(pSensei->Get_NewStudent())->Get_StudentInfo().eStudent;
	
	/* For.Player */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHASCENE, pLayerTag, TEXT("Prototype_GameObject_StudentImg"), &iImgNum)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}


CGacha_Scene * CGacha_Scene::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGacha_Scene*		pInstance = new CGacha_Scene(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CGacha_Scene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGacha_Scene::Free()
{
	__super::Free();

}
