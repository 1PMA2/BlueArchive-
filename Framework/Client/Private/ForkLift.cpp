#include "stdafx.h"
#include "..\Public\ForkLift.h"

#include "GameInstance.h"
#include "Student.h"
#include "Collider.h"

CForkLift::CForkLift(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CForkLift::CForkLift(const CForkLift & rhs)
	: CGameObject(rhs)
{
}

HRESULT CForkLift::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CForkLift::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
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

void CForkLift::Tick(_float fTimeDelta)
{
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CForkLift::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	
	Collision_ToPlayer();

}

HRESULT CForkLift::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;


	

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/
		m_pModelCom->Render(i, m_pShaderCom);
	}


#ifdef _DEBUG
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CForkLift::OnDisable()
{
}

void CForkLift::OnEnable()
{
}

HRESULT CForkLift::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CForkLift::SetUp_ShaderResource()
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


	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);

	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fRange", &pLightDesc->fRange, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CForkLift::Collision_ToPlayer()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	for (_uint i = 0; i < pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Student")); ++i)
	{
		CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student"), i);

		CCollider* pCollider = (CCollider*)pStudent->Get_Component(TEXT("Com_SPHERE"));

		if (m_pSphereCom->Collision(pCollider))
		{
			m_bUsed = true;
			m_bExit = true; //나갈 준비
		}

		if (m_bExit)
		{
			if (!m_pSphereCom->Collision(pCollider))
			{
				m_bExit = false;
				m_bUsed = false;
			}
		}
	}
}

CForkLift * CForkLift::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CForkLift*		pInstance = new CForkLift(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CForkLift");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CForkLift::Clone(void * pArg)
{
	CForkLift*		pInstance = new CForkLift(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CForkLift");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CForkLift::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);


}
