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
	m_tStudentInfo.eAnim = ANIM_KNEEZOOMFIRE;
	m_tStudentInfo.eFormation = FORMATION_SECOND;
	m_tStudentInfo.iAtk = 5;
	m_tStudentInfo.iDef = 0;
	m_tStudentInfo.iEx = 30;
	m_tStudentInfo.fExCost = 4.f;
	m_tStudentInfo.iHp = 100;
	m_tStudentInfo.iMagazine = 30;
	m_tStudentInfo.iBullet = 30;
	m_tStudentInfo.iRange = 12;
	m_tStudentInfo.iShield = 0;

	return S_OK;
}

HRESULT CMutsuki::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(180.0f);
	

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&TransformDesc)))
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
		FormationLevel_Collision();
		break;
	case LEVEL_GAMEPLAY:
		GamePlayLevel_Collision();
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

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(0.5f, 1.f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(11.2f, 11.2f, 11.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE_Obstacle"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(m_tStudentInfo.iRange, m_tStudentInfo.iRange, m_tStudentInfo.iRange);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE_Monster"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

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

HRESULT CMutsuki::FormationLevel_Collision()
{
	if (m_pAABBCom->CollisionRay())
	{ 
		if (KEY(LBUTTON, TAP))
		{
			m_bPicked = true;
		}
		
	}
	if(KEY(LBUTTON, AWAY))
		m_bPicked = false;

	__super::FormationLevel_Collision();

	return S_OK;
}

HRESULT CMutsuki::GamePlayLevel_Collision()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CCollider*			pMonsterCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Com_SPHERE"));
	if (nullptr == pMonsterCollider)
		return E_FAIL;
	
	m_bFoundMonster = m_pSphereCom->Collision(pMonsterCollider);


	CCollider* pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Obstacle"), TEXT("Com_SPHERE"));
	if (nullptr == pTargetCollider)
		return E_FAIL;
	
	m_bFoundObstacle = m_pSphereCom->Collision(pTargetCollider);


	RELEASE_INSTANCE(CGameInstance);
	
	return S_OK;
}

void CMutsuki::OnDisable()
{
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
