#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Free.h"
#include "Camera_Ex.h"
#include "Sensei.h"
#include "Student.h"


CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Student_Ex(TEXT("Layer_Student_Ex"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Student(TEXT("Layer_Student"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Trigger(TEXT("Layer_Trigger"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Cover(TEXT("Layer_Cover"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	CSensei* pSensei = CSensei::Get_Instance();

	pSensei->Tick_Cost(fTimeDelta);


	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		pSensei->Set_PreLevel(LEVEL_GAMEPLAY);

		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOBBY))))
			return;
	
		Safe_Release(pGameInstance);
	}

	Change_Camera();
}

void CLevel_GamePlay::Late_Tick(_float TimeDelta)
{
	
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임프렐이레벨임. "));

	return S_OK;
}

void CLevel_GamePlay::Change_Camera()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CSensei* pSensei = CSensei::Get_Instance();

	CCamera* pCameraMain = (CCamera*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), CAMERA_FREE);
	CCamera* pCameraEx = (CCamera*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), CAMERA_EX);

	if (nullptr != pCameraEx)
	{
		if (pSensei->Get_SenseiInfo().bEx)
		{
			ENABLE(pCameraEx);
			DISABLE(pCameraMain);
		}
		else
		{
			ENABLE(pCameraMain);
			DISABLE(pCameraEx);
		}
	}
}


HRESULT CLevel_GamePlay::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Camera_Free */
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(10.f, 5.f, 0.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 5.5f, 1.f);
	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fFovy = XMConvertToRadians(25.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Main"), &CameraDesc)))
		return E_FAIL;

	CameraDesc.fFovy = XMConvertToRadians(25.0f);
	
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Ex"), &CameraDesc)))
		return E_FAIL;
	


	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_City"))))
		return E_FAIL;

	/* For.Terrain */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	/* For.Sky */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;



	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Student(const _tchar * pLayerTag)
{	
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CSensei* pSensei = CSensei::Get_Instance();

	/* For.Player */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag,
		pSensei->Get_Student(TEXT("Aru"))->Get_StudentInfo().pName)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag,
		pSensei->Get_Student(TEXT("Mutsuki"))->Get_StudentInfo().pName)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Student_Ex(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Player */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Aru_Ex"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//for (_uint i = 0; i < 20; ++i)
	//{
	//	/* For.Monster */
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster"))))
	//		return E_FAIL;
	//}
	_vector vTranslation;


	vTranslation = XMVectorSet(0.f, 0.f, 30.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster"), &vTranslation)))
		return E_FAIL;

	vTranslation = XMVectorSet(1.f, 0.f, 31.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster"), &vTranslation)))
		return E_FAIL;

	vTranslation = XMVectorSet(2.f, 0.f, 38.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster"), &vTranslation)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Trigger(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	_vector vTranslation = XMVectorSet(0.f, 0.f, 14.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Trigger"), &vTranslation)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Cover(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_vector vTranslation = XMVectorSet(3.f, 0.f, 16.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift"), &vTranslation)))
		return E_FAIL;

	vTranslation = XMVectorSet(-1.f, 0.f, 15.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift"), &vTranslation)))
		return E_FAIL;

	vTranslation = XMVectorSet(-0.f, 0.f, 18.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift"), &vTranslation)))
		return E_FAIL;

	vTranslation = XMVectorSet(2.f, 0.f, 15.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift"), &vTranslation)))
		return E_FAIL;

	vTranslation = XMVectorSet(-1.f, 0.f, 16.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift"), &vTranslation)))
		return E_FAIL;


	vTranslation = XMVectorSet(-2.f, 0.f, 20.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift"), &vTranslation)))
		return E_FAIL;


	vTranslation = XMVectorSet(1.f, 0.f, 26.5f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift"), &vTranslation)))
		return E_FAIL;


	vTranslation = XMVectorSet(-0.5f, 0.f, 27.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift"), &vTranslation)))
		return E_FAIL;

	vTranslation = XMVectorSet(2.f, 0.f, 34.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift"), &vTranslation)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (_uint i = 0; i < 20; ++i)
	{
		/* For.Effect */
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect"))))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

}

