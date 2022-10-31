#include "stdafx.h"
#include "..\Public\Aru.h"

#include "GameInstance.h"
#include "Run_Sign.h"
#include "Run.h"
#include "Sensei.h"
#include "Formation_Idle.h"

CAru::CAru(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CStudent(pDevice, pContext)
{
}

CAru::CAru(const CAru & rhs)
	: CStudent(rhs)
{
}

HRESULT CAru::Initialize_Prototype()
{
	m_tStudentInfo.pName = TEXT("Aru");
	m_tStudentInfo.bExModel = false;
	m_tStudentInfo.eStudent = ARU;
	m_tStudentInfo.iEx = 200;
	return S_OK;
}

HRESULT CAru::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(180.0f);
	
	m_tStudentInfo.eAnim = ANIM_KNEEZOOMFIRE;
	//m_tStudentInfo.eFormation = FORMATION_FIRST;
	m_tStudentInfo.eFormation = FORMATION_END;
	m_tStudentInfo.eWeapon = SR;
	m_tStudentInfo.fFireSpeed = 0.5f;
	m_tStudentInfo.iAtk = 60;
	m_tStudentInfo.iDef = 0;
	m_tStudentInfo.fExCost = 4.f;
	m_tStudentInfo.iHp = 200;
	m_tStudentInfo.iMagazine = 5;
	m_tStudentInfo.iBullet = 5;
	m_tStudentInfo.fRange = 5.5f;
	m_tStudentInfo.fReConRange = 10.f;
	m_tStudentInfo.iShield = 0;

	m_bExReady = false;
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CSensei* pSensei = GET_SENSEI;

	if(LEVEL_GAMEPLAY == pSensei->Get_CurrentLevel())
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student_Ex"), TEXT("Aru_Ex"), this)))
			return E_FAIL;

	return S_OK;
}

void CAru::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);

}

void CAru::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

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

}

HRESULT CAru::Render()
{
	__super::Render();

	

	return S_OK;
}

HRESULT CAru::SetUp_Components()
{
	__super::SetUp_Components();


	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Aru"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAru::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}

HRESULT CAru::FormationLevel_Collision(_float fTimeDelta)
{


	return S_OK;
}

HRESULT CAru::GamePlayLevel_Collision(_float fTimeDelta)
{
	__super::GamePlayLevel_Collision(fTimeDelta);

	return S_OK;
}

void CAru::OnDisable()
{
	__super::OnDisable();
}

void CAru::OnEnable()
{
	__super::OnEnable();
}

CAru * CAru::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAru*		pInstance = new CAru(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAru");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CAru::Clone(void * pArg)
{
	CAru*		pInstance = new CAru(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAru");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAru::Free()
{
	__super::Free();
}
