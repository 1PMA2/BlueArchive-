#include "stdafx.h"
#include "..\Public\Haruka_ExBullet.h"

#include "GameInstance.h"
#include "Monster_State.h"
#include "M_Landing.h"
#include "Sensei.h"
#include "Student.h"
#include "Monster.h"
#include "Camera_Main.h"

CHaruka_ExBullet::CHaruka_ExBullet(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CHaruka_ExBullet::CHaruka_ExBullet(const CHaruka_ExBullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHaruka_ExBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHaruka_ExBullet::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	if (nullptr != pArg)
		memcpy(&m_pOwner, pArg, sizeof(CStudent*));


	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, pTransform->Get_WorldMatrix().r[2]);

	_vector vTranslation = vTranslation += XMVector3Normalize(pTransform->Get_WorldMatrix().r[2]) * 1.f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, pTransform->Get_WorldMatrix().r[3]);

	m_iDamage = m_pOwner->Get_StudentInfo().iEx;

	m_bOnce = true;
	m_fBoomAcc = 0.f;



	return S_OK;
}

void CHaruka_ExBullet::Tick(_float fTimeDelta)
{




	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());


}

void CHaruka_ExBullet::LateTick(_float fTimeDelta)
{
#ifdef _DEBUG
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
#endif // _DEBUG

	Boom();

}

HRESULT CHaruka_ExBullet::Render()
{

	/* 셰이더 전역변수에 값을 던진다. */


#ifdef _DEBUG
	m_pOBBCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CHaruka_ExBullet::OnDisable()
{
}

void CHaruka_ExBullet::OnEnable()
{
}

void CHaruka_ExBullet::Boom()
{
	Collision_ToMonster();

}

HRESULT CHaruka_ExBullet::Collision_ToMonster()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_uint iCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

	for (_uint i = 0; i < iCount; ++i)
	{
		CMonster* pMonster = (CMonster*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), i);

		CCollider* pAABB = (CCollider*)pMonster->Get_Component(TEXT("Com_AABB"));

		if (m_pOBBCom->Collision(pAABB))
		{
			pMonster->Set_MinusHp(m_iDamage);
		}

	}

	DELETE(this);

	return S_OK;
}

HRESULT CHaruka_ExBullet::SetUp_Components()
{

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, (ColliderDesc.vScale.z) * 0.5f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CHaruka_ExBullet * CHaruka_ExBullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHaruka_ExBullet*		pInstance = new CHaruka_ExBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHaruka_ExBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHaruka_ExBullet::Clone(void * pArg)
{
	CHaruka_ExBullet*		pInstance = new CHaruka_ExBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHaruka_ExBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHaruka_ExBullet::Free()
{
	__super::Free();


	Safe_Release(m_pOBBCom);
	Safe_Release(m_pRendererCom);


}