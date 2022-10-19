#include "stdafx.h"
#include "..\Public\Mutsuki.h"

#include "GameInstance.h"
#include "Run_Sign.h"
#include "Run.h"
#include "Sensei.h"
#include "Formation_Idle.h"

CMutsuki::CMutsuki(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CStudent(pDevice, pContext)
{
}

CMutsuki::CMutsuki(const CMutsuki & rhs)
	: CStudent(rhs)
{
}

HRESULT CMutsuki::Initialize_Prototype()
{
	m_tStudentInfo.pName = TEXT("Mutsuki");
	m_tStudentInfo.bExModel = false;
	m_tStudentInfo.fFireSpeed = 0.2f;
	m_tStudentInfo.eStudent = MUTSUKI;
	return S_OK;
}

HRESULT CMutsuki::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(180.0f);
	
	m_tStudentInfo.eAnim = ANIM_KNEEZOOMFIRE;
	//m_tStudentInfo.eFormation = FORMATION_SECOND;
	m_tStudentInfo.eFormation = FORMATION_END;
	m_tStudentInfo.iAtk = 15;
	m_tStudentInfo.iDef = 0;
	m_tStudentInfo.iEx = 30;
	m_tStudentInfo.fExCost = 4.f;
	m_tStudentInfo.iHp = 100;
	m_tStudentInfo.iMagazine = 10;
	m_tStudentInfo.iBullet = 10;
	m_tStudentInfo.fRange = 5.f;
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
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student_Ex"), TEXT("Mutsuki_Ex"), this)))
			return E_FAIL;

	return S_OK;
}

void CMutsuki::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);

}

void CMutsuki::LateTick(_float fTimeDelta)
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

HRESULT CMutsuki::Render()
{
	__super::Render();



	return S_OK;
}

HRESULT CMutsuki::SetUp_Components()
{
	__super::SetUp_Components();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Mutsuki"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMutsuki::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}

HRESULT CMutsuki::FormationLevel_Collision(_float fTimeDelta)
{


	__super::FormationLevel_Collision(fTimeDelta);

	return S_OK;
}

HRESULT CMutsuki::GamePlayLevel_Collision(_float fTimeDelta)
{
	__super::GamePlayLevel_Collision(fTimeDelta);
	
	
	CSensei* pSensei = GET_SENSEI;

	if (KEY(NUM2, TAP) && pSensei->Get_SenseiInfo().fCost >= m_tStudentInfo.fExCost)
	{
		pSensei->ReSet_Ex();
		pSensei->Set_ExReady(true);
		pSensei->Set_ExStudent(this);
		Set_ExReady(true);
	}



	return S_OK;
}

void CMutsuki::OnDisable()
{
	__super::OnDisable();
}

void CMutsuki::OnEnable()
{
}

CMutsuki * CMutsuki::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMutsuki*		pInstance = new CMutsuki(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMutsuki");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CMutsuki::Clone(void * pArg)
{
	CMutsuki*		pInstance = new CMutsuki(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMutsuki");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMutsuki::Free()
{
	__super::Free();
}
