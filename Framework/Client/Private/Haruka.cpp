#include "stdafx.h"
#include "..\Public\Haruka.h"

#include "GameInstance.h"
#include "Run_Sign.h"
#include "Run.h"
#include "Sensei.h"
#include "Formation_Idle.h"

CHaruka::CHaruka(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CStudent(pDevice, pContext)
{
}

CHaruka::CHaruka(const CHaruka & rhs)
	: CStudent(rhs)
{
}

HRESULT CHaruka::Initialize_Prototype()
{
	m_tStudentInfo.pName = TEXT("Haruka");
	m_tStudentInfo.bExModel = false;
	m_tStudentInfo.fFireSpeed = 0.2f;
	m_tStudentInfo.eStudent = HARUKA;
	return S_OK;
}

HRESULT CHaruka::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(180.0f);

	m_tStudentInfo.eAnim = ANIM_KNEEZOOMFIRE;
	//m_tStudentInfo.eFormation = FORMATION_THIRD;
	m_tStudentInfo.eFormation = FORMATION_END;
	m_tStudentInfo.eWeapon = SHOTGUN;
	m_tStudentInfo.iAtk = 40;
	m_tStudentInfo.iDef = 0;
	m_tStudentInfo.iEx = 30;
	m_tStudentInfo.fExCost = 5.f;
	m_tStudentInfo.iHp = 100;
	m_tStudentInfo.iMagazine = 5;
	m_tStudentInfo.iBullet = 5;
	m_tStudentInfo.fRange = 2.5f;
	m_tStudentInfo.fReConRange = 10.f;
	m_tStudentInfo.iShield = 0;

	m_bExReady = false;
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CSensei* pSensei = GET_SENSEI;

	if (LEVEL_GAMEPLAY == pSensei->Get_CurrentLevel())
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student_Ex"), TEXT("Haruka_Ex"), this)))
			return E_FAIL;

	return S_OK;
}

void CHaruka::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);

}

void CHaruka::LateTick(_float fTimeDelta)
{


	CSensei* pSensei = CSensei::Get_Instance();

	switch (pSensei->Get_CurrentLevel())
	{
	case LEVEL_FORMATION:
		FormationLevel_Collision(fTimeDelta);
		break;
	case LEVEL_GAMEPLAY:
		GamePlayLevel_Collision(fTimeDelta);
		break;
	}



	__super::LateTick(fTimeDelta);
}

HRESULT CHaruka::Render()
{
	__super::Render();



	return S_OK;
}

HRESULT CHaruka::SetUp_Components()
{
	__super::SetUp_Components();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Haruka"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHaruka::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}

HRESULT CHaruka::FormationLevel_Collision(_float fTimeDelta)
{


	__super::FormationLevel_Collision(fTimeDelta);

	return S_OK;
}

HRESULT CHaruka::GamePlayLevel_Collision(_float fTimeDelta)
{
	__super::GamePlayLevel_Collision(fTimeDelta);

	return S_OK;
}

void CHaruka::OnDisable()
{
	__super::OnDisable();
}

void CHaruka::OnEnable()
{
	__super::OnEnable();
}

CHaruka * CHaruka::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHaruka*		pInstance = new CHaruka(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHaruka");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHaruka::Clone(void * pArg)
{
	CHaruka*		pInstance = new CHaruka(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHaruka");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHaruka::Free()
{
	__super::Free();
}