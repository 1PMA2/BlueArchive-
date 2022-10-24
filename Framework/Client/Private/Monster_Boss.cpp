#include "stdafx.h"
#include "..\Public\Monster_Boss.h"

#include "GameInstance.h"
#include "Monster_State.h"
#include "Boss_Landing.h"
#include "Sensei.h"
#include "Student.h"
#include "HIerarchyNode.h"


CMonster_Boss::CMonster_Boss(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CMonster(pDevice, pContext)
{
}

CMonster_Boss::CMonster_Boss(const CMonster_Boss & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Boss::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CMonster_Boss::Initialize(void * pArg)
{
	
	m_tTransformDesc.fSpeedPerSec = 1.5f;
	m_tTransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&m_tTransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_vector vTranslation;

	if (nullptr != pArg)
		memcpy(&vTranslation, pArg, sizeof(_vector));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vTranslation);

	m_tMonsterInfo.eMonster = BOSS;
	m_tMonsterInfo.iAtk = 20;
	m_tMonsterInfo.iHp = 3000;
	m_tMonsterInfo.fRange = 6.f;

	m_pState = CBoss_Landing::Create(this);
	
	m_pBone = m_pModelCom->Find_HierarcyNode("Head_sp");
	
	return S_OK;
}

void CMonster_Boss::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);


}

void CMonster_Boss::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CMonster_Boss::Render()
{
	__super::Render();

	return S_OK;
}

void CMonster_Boss::OnDisable()
{
}

void CMonster_Boss::OnEnable()
{
}

HRESULT CMonster_Boss::SetUp_Components()
{

	__super::SetUp_Components();

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TankHihumi"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Boss::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}


CMonster_Boss * CMonster_Boss::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMonster_Boss*		pInstance = new CMonster_Boss(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster_Boss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonster_Boss::Clone(void * pArg)
{
	CMonster_Boss*		pInstance = new CMonster_Boss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster_Boss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Boss::Free()
{
	__super::Free();

}