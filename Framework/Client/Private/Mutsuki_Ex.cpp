#include "stdafx.h"
#include "..\Public\Mutsuki_Ex.h"

#include "GameInstance.h"
#include "Ex_Cutin.h"


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

	return S_OK;
}

void CMutsuki_Ex::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);

}

void CMutsuki_Ex::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CMutsuki_Ex::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CMutsuki_Ex::SetUp_Components()
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