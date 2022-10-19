#include "stdafx.h"
#include "..\Public\Haruka_Ex.h"

#include "GameInstance.h"
#include "Ex_Cutin.h"
#include "Camera.h"
#include "Sensei.h"

CHaruka_Ex::CHaruka_Ex(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CStudent(pDevice, pContext)
{
}

CHaruka_Ex::CHaruka_Ex(const CHaruka_Ex & rhs)
	: CStudent(rhs)
{
}

HRESULT CHaruka_Ex::Initialize_Prototype()
{
	m_tStudentInfo.pName = TEXT("Haruka_Ex");
	m_tStudentInfo.bExModel = true;
	m_tStudentInfo.eStudent = HARUKA;

	return S_OK;
}

HRESULT CHaruka_Ex::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	m_tStudentInfo.eAnim = ANIM_EX;
	m_tStudentInfo.eFormation = FORMATION_THIRD;


	if (FAILED(SetUp_Components()))
		return E_FAIL;




	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (nullptr != pArg)
		m_pHaruka = (CStudent*)pArg;

	m_pModelCom->Set_CurrentAnimation(0);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Haruka"), TEXT("Prototype_GameObject_Camera_Haruka"), this)))
		return E_FAIL;

	return S_OK;
}

void CHaruka_Ex::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CSensei* pSensei = GET_SENSEI;

	CCamera* pCameraEx = (CCamera*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Haruka"));

	if (m_pHaruka->Get_Ex())
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

void CHaruka_Ex::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CHaruka_Ex::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CHaruka_Ex::SetUp_Components()
{
	__super::SetUp_Components();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Haruka_Ex"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHaruka_Ex::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}

void CHaruka_Ex::OnDisable()
{
}

void CHaruka_Ex::OnEnable()
{
}

CHaruka_Ex * CHaruka_Ex::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHaruka_Ex*		pInstance = new CHaruka_Ex(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHaruka_Ex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHaruka_Ex::Clone(void * pArg)
{
	CHaruka_Ex*		pInstance = new CHaruka_Ex(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHaruka_Ex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHaruka_Ex::Free()
{
	__super::Free();
}
