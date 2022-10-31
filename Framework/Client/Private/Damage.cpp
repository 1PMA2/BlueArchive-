#include "stdafx.h"
#include "..\Public\Damage.h"

#include "GameInstance.h"
#include "Student.h"
#include "HIerarchyNode.h"

CDamage::CDamage(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CDamage::CDamage(const CDamage & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDamage::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDamage::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_tDAMAGE, pArg, sizeof(DAMAGE));

	m_vTranslation = XMVectorSet(XMVectorGetX(m_tDAMAGE.vTranslation), XMVectorGetY(m_tDAMAGE.vTranslation) + 0.7f, XMVectorGetZ(m_tDAMAGE.vTranslation), 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_vTranslation);

	m_vTranslation = XMVectorSet(XMVectorGetX(m_tDAMAGE.vTranslation), XMVectorGetY(m_tDAMAGE.vTranslation) + 1.5f, XMVectorGetZ(m_tDAMAGE.vTranslation), 1.f);
	m_iNum = m_tDAMAGE.iNum;
	return S_OK;
}

void CDamage::Tick(_float fTimeDelta)
{
	if (nullptr == m_pVIBufferCom)
		return;

	m_fTimeAcc += fTimeDelta;

	m_pTransformCom->Go_Lerp(m_vTranslation, 1, fTimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();

	_matrix vInv = pGameInstance->Get_Inv();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vInv.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vInv.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vInv.r[2]);

	m_pTransformCom->Set_Scaled(_float3(0.18f, 0.2f, 0.f));

}

void CDamage::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

	if (1.f < m_fTimeAcc)
		DELETE(this);
}

HRESULT CDamage::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(6);

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CDamage::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Damage"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDamage::SetUp_ShaderResource()
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
	if (FAILED(m_pShaderCom->Set_RawValue("g_DNum", &m_iNum, sizeof(_uint))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CDamage::OnDisable()
{
}

void CDamage::OnEnable()
{
}

CDamage * CDamage::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CDamage*		pInstance = new CDamage(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDamage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDamage::Clone(void * pArg)
{
	CDamage*		pInstance = new CDamage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDamage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamage::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

}
