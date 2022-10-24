#include "stdafx.h"
#include "..\Public\Bullet.h"

#include "GameInstance.h"
#include "Student.h"
#include "Monster.h"
#include "HIerarchyNode.h"
#include "Collider.h"

CBullet::CBullet(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBullet::CBullet(const CBullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_pOwner, pArg, sizeof(CStudent*));

	InitLook();

	return S_OK;
}

void CBullet::Tick(_float fTimeDelta)
{
	if (nullptr == m_pVIBufferCom)
		return;

	m_fTick += fTimeDelta;

	m_pTransformCom->Go_Straight(5.f * fTimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	_matrix vInv = pGameInstance->Get_Inv();

	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vInv.r[2]);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, vInv.r[1]);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, vInv.r[0]);


	Safe_Release(pGameInstance);

	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CBullet::LateTick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	for (_uint i = 0; i < pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Monster")); ++i)
	{
		CMonster* pMonster = (CMonster*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), i);

		CCollider* pCollider = (CCollider*)pMonster->Get_Component(TEXT("Com_AABB"));
		if (m_pSphereCom->Collision(pCollider))
		{
			pMonster->Set_MinusHp(m_pOwner->Get_StudentInfo().iAtk);
			DELETE(this);
			break;
		}
	}

	if(m_fTick > 2.f)
		DELETE(this);


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CBullet::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CBullet::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Muzzle"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Instance"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(0.1f, 0.1f, 0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 4)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CBullet::InitLook()
{
	CTransform* pMuzzle = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));


	CModel*		pTargetModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	m_pBonePtr = pTargetModel->Find_HierarcyNode("fire_01");

	if (nullptr == m_pBonePtr)
		return E_FAIL;

	_matrix		ParentMatrix = m_pBonePtr->Get_OffsetMatrix() * m_pBonePtr->Get_CombinedMatrix() * m_pBonePtr->Get_TransformMatrix();
	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, ParentMatrix * pMuzzle->Get_WorldMatrix());

	_vector vTranslation = XMLoadFloat4x4(&m_WorldMatrix).r[CTransform::STATE_TRANSLATION];
	_vector		vLook = pMuzzle->Get_WorldMatrix().r[2];


	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vTranslation);

	CMonster* pMonster = m_pOwner->FoundMonster();
	_vector vTarget = {};

	if (pMonster)
	{
		vTarget = pMonster->Get_MonsterTranslation();
		m_pTransformCom->LookAt(XMVectorSet(XMVectorGetX(vTarget), 0.5f, XMVectorGetZ(vTarget), 1.f));
	}
	else
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, pMuzzle->Get_WorldMatrix().r[2]);

	m_pTransformCom->Set_Scaled(_float3(0.f, 0.1f, 1.f));

	return S_OK;
}

void CBullet::OnDisable()
{
}

void CBullet::OnEnable()
{
}

CBullet * CBullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBullet*		pInstance = new CBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBullet::Clone(void * pArg)
{
	CBullet*		pInstance = new CBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

}
