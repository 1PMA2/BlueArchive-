#include "stdafx.h"
#include "..\Public\Aru_Ex.h"

#include "GameInstance.h"
#include "Ex_Cutin.h"
#include "Sensei.h"
#include "Camera.h"

CAru_Ex::CAru_Ex(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CStudent(pDevice, pContext)
{
}

CAru_Ex::CAru_Ex(const CAru_Ex & rhs)
	: CStudent(rhs)
{
}

HRESULT CAru_Ex::Initialize_Prototype()
{
	m_tStudentInfo.pName = TEXT("Aru_Ex");
	m_tStudentInfo.bExModel = true;
	m_tStudentInfo.eStudent = ARU;

	return S_OK;
}

HRESULT CAru_Ex::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	m_tStudentInfo.eAnim = ANIM_EX;
	m_tStudentInfo.eFormation = FORMATION_FIRST;

	if(nullptr != pArg)
		m_pAru = (CStudent*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	m_pModelCom->Set_CurrentAnimation(0);

	
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Aru"), TEXT("Prototype_GameObject_Camera_Aru"), this)))
		return E_FAIL;

	return S_OK;
}

void CAru_Ex::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CSensei* pSensei = GET_SENSEI;

	CCamera* pCameraEx = (CCamera*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Aru"));

	if (m_pAru->Get_Ex())
	{
		ENABLE(pCameraEx);
		m_pModelCom->Play_Animation(fTimeDelta);
	}
	else
	{
		DISABLE(pCameraEx);
		m_pModelCom->ResetAnimation();
	}


	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CAru_Ex::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CAru_Ex::Render()
{
	CSensei* pSensei = GET_SENSEI;

	if (nullptr != pSensei->Get_ExStudent())
	{
		if (pSensei->Get_ExStudent()->Get_StudentInfo().eStudent == m_tStudentInfo.eStudent)
		{
			__super::Render();
		}
	}


	return S_OK;
}

HRESULT CAru_Ex::SetUp_Components()
{
	__super::SetUp_Components();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Aru_Ex"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAru_Ex::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}

void CAru_Ex::OnDisable()
{
}

void CAru_Ex::OnEnable()
{
}

CAru_Ex * CAru_Ex::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAru_Ex*		pInstance = new CAru_Ex(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAru_Ex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAru_Ex::Clone(void * pArg)
{
	CAru_Ex*		pInstance = new CAru_Ex(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAru_Ex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAru_Ex::Free()
{
	__super::Free();
}