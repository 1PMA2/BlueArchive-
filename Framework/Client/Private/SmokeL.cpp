#include "stdafx.h"
#include "..\Public\SmokeL.h"

#include "GameInstance.h"
#include "Student.h"
#include "HIerarchyNode.h"

CSmokeL::CSmokeL(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CSmokeL::CSmokeL(const CSmokeL & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSmokeL::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSmokeL::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 0.2f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_vTranslation, pArg, sizeof(_vector));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(XMVectorGetX(m_vTranslation), XMVectorGetY(m_vTranslation) + 1.5f, XMVectorGetZ(m_vTranslation), 1.f));


	InitLook();

	return S_OK;
}

void CSmokeL::Tick(_float fTimeDelta)
{
	if (nullptr == m_pVIBufferCom)
		return;

	m_pTransformCom->Go_Straight(fTimeDelta);

	m_fFrame += 20.f * fTimeDelta;


	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	_matrix vInv = pGameInstance->Get_Inv();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vInv.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vInv.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vInv.r[2]);

	//m_pTransformCom->Set_Scaled(_float3(4.f, 4.f, 4.f));

	Safe_Release(pGameInstance);

	if (m_fFrame > 16.f)
	{
		DELETE(this);
	}
}

void CSmokeL::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CSmokeL::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(4);

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CSmokeL::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_EXP"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSmokeL::SetUp_ShaderResource()
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
	if (FAILED(m_pShaderCom->Set_RawValue("g_Frame", &m_fFrame, sizeof(_int))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Large", &m_bLarge, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CSmokeL::InitLook()
{
	


	return S_OK;
}

void CSmokeL::OnDisable()
{
}

void CSmokeL::OnEnable()
{
}

CSmokeL * CSmokeL::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSmokeL*		pInstance = new CSmokeL(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSmokeL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSmokeL::Clone(void * pArg)
{
	CSmokeL*		pInstance = new CSmokeL(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSmokeL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSmokeL::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

}

