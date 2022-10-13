#include "stdafx.h"
#include "..\Public\Aru_ExBullet.h"

#include "GameInstance.h"
#include "Monster_State.h"
#include "M_Landing.h"
#include "Sensei.h"
#include "Student.h"
#include "Monster.h"

CAru_ExBullet::CAru_ExBullet(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CAru_ExBullet::CAru_ExBullet(const CAru_ExBullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CAru_ExBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAru_ExBullet::Initialize(void * pArg)
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


	return S_OK;
}

void CAru_ExBullet::Tick(_float fTimeDelta)
{
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());

	CSensei* pSensei = GET_SENSEI;


	if (m_pTarget)
	{
		if (m_bOnce)
		{
			m_pTarget->Set_MinusHp(200);
			m_bOnce = false;
		}
	}

	m_vTranslation = pSensei->Get_LockonVector();

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_vTranslation);

	m_fBoomAcc += fTimeDelta;

}

void CAru_ExBullet::LateTick(_float fTimeDelta)
{
	if (1.5f < m_fBoomAcc)
	{
		m_fBoomAcc = 0.f;
		Collision_ToMonster();
	}


}

HRESULT CAru_ExBullet::Render()
{

	/* 셰이더 전역변수에 값을 던진다. */


#ifdef _DEBUG
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CAru_ExBullet::OnDisable()
{
}

void CAru_ExBullet::OnEnable()
{
}

HRESULT CAru_ExBullet::Collision_ToMonster()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_uint iCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

	for (_uint i = 0; i < iCount; ++i)
	{
		CMonster* pMonster = (CMonster*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), i);

		CCollider* pSphere = (CCollider*)pMonster->Get_Component(TEXT("Com_SPHERE"));

		if (m_pSphereCom->Collision(pSphere))
		{
			_vector vTranslation = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
			pMonster->Set_MinusHp(200);

		}

	}

	DELETE(this);

	return S_OK;
}

HRESULT CAru_ExBullet::SetUp_Components()
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

CAru_ExBullet * CAru_ExBullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAru_ExBullet*		pInstance = new CAru_ExBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAru_ExBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAru_ExBullet::Clone(void * pArg)
{
	CAru_ExBullet*		pInstance = new CAru_ExBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAru_ExBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAru_ExBullet::Free()
{
	__super::Free();


	Safe_Release(m_pSphereCom);
	Safe_Release(m_pRendererCom);


}