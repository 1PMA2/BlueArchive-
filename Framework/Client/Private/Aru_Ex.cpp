#include "stdafx.h"
#include "..\Public\Aru_Ex.h"

#include "GameInstance.h"
#include "Run_Sign.h"
#include "Run.h"

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
	return S_OK;
}

HRESULT CAru_Ex::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	m_eStudentInfo.strName = TEXT("Aru_Ex");
	m_eStudentInfo.bExModel = true;
	m_eStudentInfo.fFireSpeed = 0.5f;
	m_eStudentInfo.eAnim = ANIM_EX;
	m_eStudentInfo.eFormation = FORMATION_FIRST_EX;
	m_eStudentInfo.iAtk = 10;
	m_eStudentInfo.iDef = 0;
	m_eStudentInfo.iEx = 50;
	m_eStudentInfo.iExCost = 4;
	m_eStudentInfo.iHp = 100;
	m_eStudentInfo.iMagazine = 5;
	m_eStudentInfo.iRange = 10;
	m_eStudentInfo.iShield = 0;


	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pState = CRun::Create(this);

	return S_OK;
}

void CAru_Ex::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);

}

void CAru_Ex::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CAru_Ex::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CAru_Ex::SetUp_Components()
{
	__super::SetUp_Components();

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.2f, 1.2f, 1.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

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