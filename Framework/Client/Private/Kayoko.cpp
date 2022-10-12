#include "stdafx.h"
#include "..\Public\Kayoko.h"

#include "GameInstance.h"
#include "Run_Sign.h"
#include "Run.h"
#include "Sensei.h"
#include "Formation_Idle.h"

CKayoko::CKayoko(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CStudent(pDevice, pContext)
{
}

CKayoko::CKayoko(const CKayoko & rhs)
	: CStudent(rhs)
{
}

HRESULT CKayoko::Initialize_Prototype()
{
	m_tStudentInfo.pName = TEXT("Kayoko");
	m_tStudentInfo.bExModel = false;
	m_tStudentInfo.fFireSpeed = 0.2f;
	m_tStudentInfo.eStudent = KAYOKO;
	return S_OK;
}

HRESULT CKayoko::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(180.0f);

	m_tStudentInfo.eAnim = ANIM_KNEEZOOMFIRE;
	m_tStudentInfo.eFormation = FORMATION_THIRD;
	m_tStudentInfo.iAtk = 10;
	m_tStudentInfo.iDef = 0;
	m_tStudentInfo.iEx = 30;
	m_tStudentInfo.fExCost = 6.f;
	m_tStudentInfo.iHp = 100;
	m_tStudentInfo.iMagazine = 10;
	m_tStudentInfo.iBullet = 10;
	m_tStudentInfo.fRange = 2.f;
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
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student_Ex"), TEXT("Kayoko_Ex"), this)))
			return E_FAIL;

	return S_OK;
}

void CKayoko::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);

}

void CKayoko::LateTick(_float fTimeDelta)
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

HRESULT CKayoko::Render()
{
	__super::Render();



	return S_OK;
}

HRESULT CKayoko::SetUp_Components()
{
	__super::SetUp_Components();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Kayoko"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CKayoko::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}

HRESULT CKayoko::FormationLevel_Collision(_float fTimeDelta)
{


	__super::FormationLevel_Collision(fTimeDelta);

	return S_OK;
}

HRESULT CKayoko::GamePlayLevel_Collision(_float fTimeDelta)
{
	__super::GamePlayLevel_Collision(fTimeDelta);


	CSensei* pSensei = GET_SENSEI;

	if (KEY(NUM3, TAP) && pSensei->Get_SenseiInfo().fCost >= m_tStudentInfo.fExCost)
	{
		pSensei->Set_ExReady();
		pSensei->Set_ExStudent(this);
		Set_ExReady(true);
	}



	return S_OK;
}

void CKayoko::OnDisable()
{
}

void CKayoko::OnEnable()
{
}

CKayoko * CKayoko::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CKayoko*		pInstance = new CKayoko(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CKayoko");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CKayoko::Clone(void * pArg)
{
	CKayoko*		pInstance = new CKayoko(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CKayoko");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKayoko::Free()
{
	__super::Free();
}