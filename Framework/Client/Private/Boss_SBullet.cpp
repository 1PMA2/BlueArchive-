#include "stdafx.h"
#include "..\Public\Boss_SBullet.h"

#include "GameInstance.h"
#include "Monster_State.h"
#include "M_Landing.h"
#include "Sensei.h"
#include "Student.h"
#include "Monster.h"
#include "Camera_Main.h"

CBoss_SBullet::CBoss_SBullet(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBoss_SBullet::CBoss_SBullet(const CBoss_SBullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBoss_SBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_SBullet::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	if (nullptr != pArg)
		memcpy(&m_pOwner, pArg, sizeof(CMonster*));

	m_bOnce = true;
	m_fBoomAcc = 0.f;

	CStudent* pTarget = m_pOwner->Get_RandomTarget();

	if (pTarget)
		m_vTranslation = pTarget->Get_StudentTranslation();
	else
		m_vTranslation = m_pOwner->Get_RandomTargetTranslation();

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_pOwner->Get_MonsterTranslation());

	return S_OK;
}

void CBoss_SBullet::Tick(_float fTimeDelta)
{

	CSensei* pSensei = GET_SENSEI;

	/*
	if (m_pTarget)
	{
	if (m_bOnce)
	{
	m_pTarget->Set_MinusHp(220);
	m_bOnce = false;
	}
	}*/


	m_pTransformCom->LookAt(m_vTranslation);

	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_fBoomAcc += fTimeDelta;

}

void CBoss_SBullet::LateTick(_float fTimeDelta)
{
#ifdef _DEBUG
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
#endif // _DEBUG

	Boom();

}

HRESULT CBoss_SBullet::Render()
{

	/* 셰이더 전역변수에 값을 던진다. */


#ifdef _DEBUG
	m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CBoss_SBullet::OnDisable()
{
}

void CBoss_SBullet::OnEnable()
{
}

void CBoss_SBullet::Boom()
{


	if (1.7f < m_fBoomAcc)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		dynamic_cast<CCamera_Main*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera")))->Shaking_Camera(true, 0.07f);

		m_fBoomAcc = 0.f;
		Collision_ToMonster();
	}
}

HRESULT CBoss_SBullet::Collision_ToMonster()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_uint iCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Student"));

	for (_uint i = 0; i < iCount; ++i)
	{
		CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student"), i);

		CCollider* pSphere = (CCollider*)pStudent->Get_Component(TEXT("Com_SPHERE"));

		if (m_pAABBCom->Collision(pSphere))
		{
			int i = 0;
			
		}

	}


	DELETE(this);

	return S_OK;
}

HRESULT CBoss_SBullet::SetUp_Components()
{

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.f, 2.f, 8.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, (ColliderDesc.vScale.z) * 0.5f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CBoss_SBullet * CBoss_SBullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_SBullet*		pInstance = new CBoss_SBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBoss_SBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBoss_SBullet::Clone(void * pArg)
{
	CBoss_SBullet*		pInstance = new CBoss_SBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBoss_SBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_SBullet::Free()
{
	__super::Free();


	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);


}
