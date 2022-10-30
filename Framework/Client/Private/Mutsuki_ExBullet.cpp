#include "stdafx.h"
#include "..\Public\Mutsuki_ExBullet.h"

#include "GameInstance.h"
#include "Monster_State.h"
#include "M_Landing.h"
#include "Sensei.h"
#include "Student.h"
#include "Monster.h"
#include "Camera_Main.h"

CMutsuki_ExBullet::CMutsuki_ExBullet(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CMutsuki_ExBullet::CMutsuki_ExBullet(const CMutsuki_ExBullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMutsuki_ExBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMutsuki_ExBullet::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	m_bOnce = true;

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	if (nullptr != pArg)
		memcpy(&m_pTarget, pArg, sizeof(CMonster*));

	CSensei* pSensei = GET_SENSEI;

	CStudent* pMutsuki = pSensei->Get_Student(TEXT("Mutsuki"));

	m_iDamage = pMutsuki->Get_StudentInfo().iEx;

	m_bOnce = true;
	m_fBoomAcc = 0.f;

	return S_OK;
}

void CMutsuki_ExBullet::Tick(_float fTimeDelta)
{
	CSensei* pSensei = GET_SENSEI;

	m_vTranslation = pSensei->Get_LockonVector();

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_vTranslation);

	m_pSphereACom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphereBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphereCCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_fBoomAcc += fTimeDelta;



}

void CMutsuki_ExBullet::LateTick(_float fTimeDelta)
{

#ifdef _DEBUG
	//m_pRendererCom->Add_DebugRenderGroup(m_pSphereACom);
	//m_pRendererCom->Add_DebugRenderGroup(m_pSphereBCom);
	//m_pRendererCom->Add_DebugRenderGroup(m_pSphereCCom);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
#endif // _DEBUG

	Boom();

}

HRESULT CMutsuki_ExBullet::Render()
{

	/* ���̴� ���������� ���� ������. */
#ifdef _DEBUG
	m_pSphereACom->Render();
	m_pSphereBCom->Render();
	m_pSphereCCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CMutsuki_ExBullet::OnDisable()
{
}

void CMutsuki_ExBullet::OnEnable()
{
}

HRESULT CMutsuki_ExBullet::Collision_ToMonster()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_uint iCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

	for (_uint i = 0; i < iCount; ++i)
	{
		CMonster* pMonster = (CMonster*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), i);

		CCollider* pAABB = (CCollider*)pMonster->Get_Component(TEXT("Com_AABB"));

		if (m_pSphereACom->Collision(pAABB))
		{
			pMonster->Set_MinusHp(m_iDamage);
		}
		if (m_pSphereBCom->Collision(pAABB))
		{
			pMonster->Set_MinusHp(m_iDamage);
		}
		if (m_pSphereCCom->Collision(pAABB))
		{
			pMonster->Set_MinusHp(m_iDamage);
		}

	}

	return S_OK;
}

HRESULT CMutsuki_ExBullet::SetUp_Components()
{

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(-ColliderDesc.vScale.x, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHEREA"), (CComponent**)&m_pSphereACom, &ColliderDesc)))
		return E_FAIL;


	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHEREB"), (CComponent**)&m_pSphereBCom, &ColliderDesc)))
		return E_FAIL;


	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(ColliderDesc.vScale.x, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHEREC"), (CComponent**)&m_pSphereCCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CMutsuki_ExBullet::Boom()
{
	if (2.5f < m_fBoomAcc)
	{
		m_fBoomAcc = 0.f;
		Collision_ToMonster();

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		dynamic_cast<CCamera_Main*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera")))->Shaking_Camera(true);

		_vector vTranslation
			;
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_SmokeL"), &m_vTranslation);

		vTranslation = XMVectorSet(XMVectorGetX(m_vTranslation) - 1.5f, 0.f, XMVectorGetZ(m_vTranslation), 1.f);
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_SmokeL"), &vTranslation);

		vTranslation = XMVectorSet(XMVectorGetX(m_vTranslation) + 1.5f, 0.f, XMVectorGetZ(m_vTranslation), 1.f);
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_SmokeL"), &vTranslation);
		DELETE(this);
	}
	
}

CMutsuki_ExBullet * CMutsuki_ExBullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMutsuki_ExBullet*		pInstance = new CMutsuki_ExBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMutsuki_ExBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMutsuki_ExBullet::Clone(void * pArg)
{
	CMutsuki_ExBullet*		pInstance = new CMutsuki_ExBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMutsuki_ExBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMutsuki_ExBullet::Free()
{
	__super::Free();


	Safe_Release(m_pSphereACom);
	Safe_Release(m_pSphereBCom);
	Safe_Release(m_pSphereCCom);
	Safe_Release(m_pRendererCom);


}