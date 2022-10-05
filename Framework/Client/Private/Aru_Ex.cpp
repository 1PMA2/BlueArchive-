#include "stdafx.h"
#include "..\Public\Aru_Ex.h"

#include "GameInstance.h"
#include "Ex_Cutin.h"

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

	
	m_tStudentInfo.fFireSpeed = 0.5f;
	m_tStudentInfo.eAnim = ANIM_EX;
	m_tStudentInfo.eFormation = FORMATION_FIRST;
	m_tStudentInfo.iAtk = 10;
	m_tStudentInfo.iDef = 0;
	m_tStudentInfo.iEx = 50;
	m_tStudentInfo.fExCost = 4.f;
	m_tStudentInfo.iHp = 100;
	m_tStudentInfo.iMagazine = 5;
	m_tStudentInfo.iBullet = 5;
	m_tStudentInfo.fRange = 10;
	m_tStudentInfo.iShield = 0;


	if (FAILED(SetUp_Components()))
		return E_FAIL;




	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
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