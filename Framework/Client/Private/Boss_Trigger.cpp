#include "stdafx.h"
#include "..\Public\Boss_Trigger.h"

#include "GameInstance.h"
#include "Monster_State.h"
#include "M_Landing.h"
#include "Sensei.h"
#include "Student.h"

CBoss_Trigger::CBoss_Trigger(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBoss_Trigger::CBoss_Trigger(const CBoss_Trigger & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBoss_Trigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Trigger::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_vTranslation, pArg, sizeof(_vector));


	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_vTranslation);


	return S_OK;
}

void CBoss_Trigger::Tick(_float fTimeDelta)
{
	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CBoss_Trigger::LateTick(_float fTimeDelta)
{
	Collision_ToStudent();


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (true == pGameInstance->isIn_Frustum_InWorldSpace(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 2.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}

	RELEASE_INSTANCE(CGameInstance);

}

HRESULT CBoss_Trigger::Render()
{

	/* 셰이더 전역변수에 값을 던진다. */

	
	#ifdef _DEBUG
		m_pAABBCom->Render();
	#endif // _DEBUG

	return S_OK;
}

void CBoss_Trigger::OnDisable()
{
}

void CBoss_Trigger::OnEnable()
{
}

HRESULT CBoss_Trigger::Collision_ToStudent()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_uint iStudentCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Student"));

	for (_uint i = 0; i < iStudentCount; ++i)
	{
		CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student"), i);

		CCollider* pSphere = (CCollider*)pStudent->Get_Component(TEXT("Com_SPHERE"));

		if (m_pAABBCom->Collision(pSphere))
		{
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Warning"))))
				return E_FAIL;

			_vector vTranslation = XMVectorSet(XMVectorGetX(m_vTranslation), XMVectorGetY(m_vTranslation), XMVectorGetZ(m_vTranslation) + 8.f, 1.f);

			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_TankHihumi"), &vTranslation)))
				return E_FAIL;

			DELETE(this);
			break;
		}

	}

	return S_OK;
}

HRESULT CBoss_Trigger::SetUp_Components()
{

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(15.f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CBoss_Trigger * CBoss_Trigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_Trigger*		pInstance = new CBoss_Trigger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBoss_Trigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBoss_Trigger::Clone(void * pArg)
{
	CBoss_Trigger*		pInstance = new CBoss_Trigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBoss_Trigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Trigger::Free()
{
	__super::Free();


	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);


}