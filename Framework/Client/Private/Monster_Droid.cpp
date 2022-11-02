#include "stdafx.h"
#include "..\Public\Monster_Droid.h"

#include "GameInstance.h"
#include "Monster_State.h"
#include "M_Landing.h"
#include "Sensei.h"
#include "Student.h"

CMonster_Droid::CMonster_Droid(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CMonster(pDevice, pContext)
{
}

CMonster_Droid::CMonster_Droid(const CMonster_Droid & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Droid::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CMonster_Droid::Initialize(void * pArg)
{
	m_tTransformDesc.fSpeedPerSec = 1.5f;
	m_tTransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tTransformDesc)))
		return E_FAIL;

	_vector vTranslation;

	if (nullptr != pArg)
		memcpy(&vTranslation, pArg, sizeof(_vector));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vTranslation);


	m_tMonsterInfo.eMonster = DROID;
	m_tMonsterInfo.iAtk = 20;
	m_tMonsterInfo.iHp = 300;
	m_tMonsterInfo.fRange = 4.f;

	m_pState = CM_Landing::Create(this);

	return S_OK;
}

void CMonster_Droid::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);


}

void CMonster_Droid::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CMonster_Droid::Render()
{
	__super::Render();

	return S_OK;
}

void CMonster_Droid::OnDisable()
{
}

void CMonster_Droid::OnEnable()
{
}

HRESULT CMonster_Droid::SetUp_Components()
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

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Droid"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Droid::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}


CMonster_Droid * CMonster_Droid::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMonster_Droid*		pInstance = new CMonster_Droid(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster_Droid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonster_Droid::Clone(void * pArg)
{
	CMonster_Droid*		pInstance = new CMonster_Droid(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster_Droid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Droid::Free()
{
	__super::Free();

}