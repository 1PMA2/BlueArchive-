#include "stdafx.h"
#include "..\Public\Arona.h"

#include "GameInstance.h"
#include "BackGround.h"

CArona::CArona(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CArona::CArona(const CArona & rhs)
	: CGameObject(rhs)
{
}

HRESULT CArona::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CArona::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));


	m_pModelCom->Set_CurrentAnimation(0);

	return S_OK;
}

void CArona::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);


}

void CArona::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CArona::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;


	if (58 >= m_pModelCom->Get_CurrentKeyFrame())
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 0, aiTextureType_DIFFUSE)))
			return E_FAIL;
		m_pModelCom->Render(0, m_pShaderCom, 0, "g_Bones");
	}
	else
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 3, aiTextureType_DIFFUSE)))
			return E_FAIL;
		m_pModelCom->Render(3, m_pShaderCom, 0, "g_Bones");
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 4, aiTextureType_DIFFUSE)))
			return E_FAIL;
		m_pModelCom->Render(4, m_pShaderCom, 0, "g_Bones");
	}

	if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 1, aiTextureType_DIFFUSE)))
		return E_FAIL;
	m_pModelCom->Render(1, m_pShaderCom, 0, "g_Bones");

	if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 2, aiTextureType_DIFFUSE)))
		return E_FAIL;
	m_pModelCom->Render(2, m_pShaderCom, 0, "g_Bones");

	if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 5, aiTextureType_DIFFUSE)))
		return E_FAIL;
	m_pModelCom->Render(5, m_pShaderCom, 0, "g_Bones");

	if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 6, aiTextureType_DIFFUSE)))
		return E_FAIL;
	m_pModelCom->Render(6, m_pShaderCom, 0, "g_Bones");

	if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 7, aiTextureType_DIFFUSE)))
		return E_FAIL;
	m_pModelCom->Render(7, m_pShaderCom, 0, "g_Bones");

	if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 8, aiTextureType_DIFFUSE)))
		return E_FAIL;
	m_pModelCom->Render(8, m_pShaderCom, 0, "g_Bones");

	if (42 < m_pModelCom->Get_CurrentKeyFrame() && 59 > m_pModelCom->Get_CurrentKeyFrame())
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 9, aiTextureType_DIFFUSE)))
			return E_FAIL;
		m_pModelCom->Render(9, m_pShaderCom, 0, "g_Bones");
	}
	else if (42 >= m_pModelCom->Get_CurrentKeyFrame())
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 10, aiTextureType_DIFFUSE)))
			return E_FAIL;
		m_pModelCom->Render(10, m_pShaderCom, 0, "g_Bones");
	}


	if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 11, aiTextureType_DIFFUSE)))
		return E_FAIL;
	m_pModelCom->Render(11, m_pShaderCom, 0, "g_Bones");

	
	// 0 번 emoface
	// 3 번 눈알
	// 4 번 눈알
	// 9 번 ><
	// 10번 - -

	return S_OK;
}

void CArona::OnDisable()
{
}

void CArona::OnEnable()
{
}

HRESULT CArona::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, -0.7f);

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Arona"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CArona::SetUp_ShaderResource()
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

CArona * CArona::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CArona*		pInstance = new CArona(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArona");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CArona::Clone(void * pArg)
{
	CArona*		pInstance = new CArona(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CArona");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArona::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);


}