#include "stdafx.h"
#include "..\Public\Boss_Bullet.h"

#include "GameInstance.h"
#include "Monster_State.h"
#include "M_Landing.h"
#include "Sensei.h"
#include "Student.h"
#include "Monster.h"
#include "Camera_Main.h"

CBoss_Bullet::CBoss_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBoss_Bullet::CBoss_Bullet(const CBoss_Bullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBoss_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Bullet::Initialize(void * pArg)
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

	return S_OK;
}

void CBoss_Bullet::Tick(_float fTimeDelta)
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
	CStudent* pTarget = m_pOwner->Get_FoundStudent();

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, pTarget->Get_StudentTranslation());

	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_fBoomAcc += fTimeDelta;

}

void CBoss_Bullet::LateTick(_float fTimeDelta)
{
#ifdef _DEBUG
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
#endif // _DEBUG

	Boom();

}

HRESULT CBoss_Bullet::Render()
{

	/* 셰이더 전역변수에 값을 던진다. */


#ifdef _DEBUG
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CBoss_Bullet::OnDisable()
{
}

void CBoss_Bullet::OnEnable()
{
}

void CBoss_Bullet::Boom()
{


	if (0.5f < m_fBoomAcc)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		dynamic_cast<CCamera_Main*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera")))->Shaking_Camera(true);

		m_fBoomAcc = 0.f;
		Collision_ToMonster();
	}
}

HRESULT CBoss_Bullet::Collision_ToMonster()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_uint iCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Student"));

	for (_uint i = 0; i < iCount; ++i)
	{
		CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student"), i);

		CCollider* pSphere = (CCollider*)pStudent->Get_Component(TEXT("Com_SPHERE"));

		if (m_pSphereCom->Collision(pSphere))
		{
			
			int i = 0;
		}

	}


	DELETE(this);

	return S_OK;
}

HRESULT CBoss_Bullet::SetUp_Components()
{

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CBoss_Bullet * CBoss_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_Bullet*		pInstance = new CBoss_Bullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBoss_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBoss_Bullet::Clone(void * pArg)
{
	CBoss_Bullet*		pInstance = new CBoss_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBoss_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Bullet::Free()
{
	__super::Free();


	Safe_Release(m_pSphereCom);
	Safe_Release(m_pRendererCom);


}