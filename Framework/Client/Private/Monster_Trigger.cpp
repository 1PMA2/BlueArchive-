#include "stdafx.h"
#include "..\Public\Monster_Trigger.h"

#include "GameInstance.h"
#include "Monster_State.h"
#include "M_Landing.h"
#include "Sensei.h"
#include "Student.h"

CMonster_Trigger::CMonster_Trigger(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CMonster_Trigger::CMonster_Trigger(const CMonster_Trigger & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster_Trigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster_Trigger::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	
	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_vector vTranslation;

	if (nullptr != pArg)
		memcpy(&vTranslation, pArg, sizeof(_vector));


	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vTranslation);


	return S_OK;
}

void CMonster_Trigger::Tick(_float fTimeDelta)
{
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CMonster_Trigger::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CMonster_Trigger::Render()
{

	/* 셰이더 전역변수에 값을 던진다. */


#ifdef _DEBUG
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CMonster_Trigger::OnDisable()
{
}

void CMonster_Trigger::OnEnable()
{
}

HRESULT CMonster_Trigger::SetUp_Components()
{

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(5.f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CMonster_Trigger * CMonster_Trigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMonster_Trigger*		pInstance = new CMonster_Trigger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster_Trigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonster_Trigger::Clone(void * pArg)
{
	CMonster_Trigger*		pInstance = new CMonster_Trigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster_Trigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Trigger::Free()
{
	__super::Free();


	Safe_Release(m_pSphereCom);
	Safe_Release(m_pRendererCom);


}