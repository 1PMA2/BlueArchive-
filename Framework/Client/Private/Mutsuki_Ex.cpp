#include "stdafx.h"
#include "..\Public\Mutsuki_Ex.h"

#include "GameInstance.h"
#include "Ex_Cutin.h"
#include "Camera.h"
#include "Sensei.h"

CMutsuki_Ex::CMutsuki_Ex(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CStudent(pDevice, pContext)
{
}

CMutsuki_Ex::CMutsuki_Ex(const CMutsuki_Ex & rhs)
	: CStudent(rhs)
{
}

HRESULT CMutsuki_Ex::Initialize_Prototype()
{
	m_tStudentInfo.pName = TEXT("Mutsuki_Ex");
	m_tStudentInfo.bExModel = true;
	m_tStudentInfo.eStudent = MUTSUKI;

	return S_OK;
}

HRESULT CMutsuki_Ex::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	m_tStudentInfo.eAnim = ANIM_EX;
	m_tStudentInfo.eFormation = FORMATION_SECOND;


	if (FAILED(SetUp_Components()))
		return E_FAIL;




	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (nullptr != pArg)
		m_pMutsuki = (CStudent*)pArg;

	m_pModelCom->Set_CurrentAnimation(1);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Mutsuki"), TEXT("Prototype_GameObject_Camera_Mutsuki"), this)))
		return E_FAIL;

	return S_OK;
}

void CMutsuki_Ex::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CSensei* pSensei = GET_SENSEI;

	CCamera* pCameraEx = (CCamera*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Mutsuki"));

	if (m_pMutsuki->Get_Ex())
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

void CMutsuki_Ex::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CMutsuki_Ex::Render()
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

HRESULT CMutsuki_Ex::SetUp_Components()
{
	__super::SetUp_Components();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Mutsuki_Ex"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMutsuki_Ex::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}

void CMutsuki_Ex::OnDisable()
{
}

void CMutsuki_Ex::OnEnable()
{
}

CMutsuki_Ex * CMutsuki_Ex::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMutsuki_Ex*		pInstance = new CMutsuki_Ex(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMutsuki_Ex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMutsuki_Ex::Clone(void * pArg)
{
	CMutsuki_Ex*		pInstance = new CMutsuki_Ex(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMutsuki_Ex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMutsuki_Ex::Free()
{
	__super::Free();
}