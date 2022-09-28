#include "stdafx.h"
#include "..\Public\Level_Formation.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Free.h"
#include "Camera_Ex.h"
#include "Sensei.h"
#include "Student.h"


CLevel_Formation::CLevel_Formation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Formation::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Formation_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Formation_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Student(TEXT("Layer_Formation_Student"))))
		return E_FAIL;


	
	

	return S_OK;
}

void CLevel_Formation::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (KEY(ENTER,TAP))
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CSensei* pSensei = GET_SENSEI;

		pSensei->Set_PreLevel(LEVEL_FORMATION);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOBBY))))
			return;

		Safe_Release(pGameInstance);
	}


}

void CLevel_Formation::Late_Tick(_float TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CSensei* pSensei = GET_SENSEI;

	pSensei->Set_FormationInfo();

	for (_uint i = 0; i < pSensei->Get_FormationStudentsNum(); ++i)
	{
		CStudent* pStudent = pSensei->Get_FormationStudents(i);

		CTransform* pTransform = (CTransform*)pStudent->Get_Component(TEXT("Com_Transform"));

		if (0 < pSensei->Get_FormationInfoSize())
		{
			for (_uint i = 0; i < pSensei->Get_FormationInfoSize(); ++i)
			{
				if (pSensei->Get_FormationInfo(i).iIndex == pStudent->Get_StudentInfo().iIndex)
				{
					switch (pSensei->Get_FormationInfo(i).eFormation)
					{
					case FORMATION_FIRST:
						m_vPreTranslation = XMVectorSet(1.5f, 0.f, 0.f, 1.f);
						break;
					case FORMATION_SECOND:
						m_vPreTranslation = XMVectorSet(0.5f, 0.f, 0.f, 1.f);
						break;
					case FORMATION_THIRD:
						m_vPreTranslation = XMVectorSet(-0.5f, 0.f, 0.f, 1.f);
						break;
					case FORMATION_FOURTH:
						m_vPreTranslation = XMVectorSet(-1.5f, 0.f, 0.f, 1.f);
						break;
					}
				}
			}
		}

	if (pStudent->Is_Picked())
	{
		_float4 fOut;
		pGameInstance->Picking((CVIBuffer*)pGameInstance->Get_Component(LEVEL_FORMATION, TEXT("Layer_Formation_BackGround"), TEXT("Com_VIBuffer"), 0),
			(CTransform*)pGameInstance->Get_Component(LEVEL_FORMATION, TEXT("Layer_Formation_BackGround"), TEXT("Com_Transform"), 0), &fOut);

		fOut.y -= 0.5f; // offset
		pTransform->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&fOut));

		if (-0.5f < fOut.y && 1.f > fOut.y)
		{
			if (1.f <= fOut.x)
			{
				pStudent->Set_Formation(FORMATION_FIRST);
				m_vPreTranslation = XMVectorSet(1.5f, 0.f, 0.f, 1.f);
			}
			else if (1.f > fOut.x && 0.f <= fOut.x)
			{
				pStudent->Set_Formation(FORMATION_SECOND);
				m_vPreTranslation = XMVectorSet(0.5f, 0.f, 0.f, 1.f);
			}
			else if (0.f > fOut.x && -1.f <= fOut.x)
			{
				pStudent->Set_Formation(FORMATION_THIRD);
				m_vPreTranslation = XMVectorSet(-0.5f, 0.f, 0.f, 1.f);
			}
			else if (-1.f > fOut.x && -2.f <= fOut.x)
			{
				pStudent->Set_Formation(FORMATION_FOURTH);
				m_vPreTranslation = XMVectorSet(-1.5f, 0.f, 0.f, 1.f);
			}
		}  //포메이션 위치

	}
	else
		pTransform->Set_State(CTransform::STATE_TRANSLATION, m_vPreTranslation); //이전위치로

	}

}

HRESULT CLevel_Formation::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("Formation. "));

	return S_OK;
}


HRESULT CLevel_Formation::Ready_Lights()
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

HRESULT CLevel_Formation::Ready_Layer_BackGround(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Terrain */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FORMATION, pLayerTag, TEXT("Prototype_GameObject_Formation_Terrain"))))
	return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Formation::Ready_Layer_Camera(const _tchar * pLayerTag)
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



HRESULT CLevel_Formation::Ready_Layer_Student(const _tchar * pLayerTag)
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


CLevel_Formation * CLevel_Formation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Formation*		pInstance = new CLevel_Formation(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Formation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Formation::Free()
{
	__super::Free();

}
