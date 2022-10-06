#include "stdafx.h"
#include "..\Public\Formation_Terrain.h"

#include "GameInstance.h"

CFormation_Terrain::CFormation_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CFormation_Terrain::CFormation_Terrain(const CFormation_Terrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CFormation_Terrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFormation_Terrain::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 1.f), XMConvertToRadians(90.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-5.f, 5.f, 0.f, 1.f));

	return S_OK;
}

void CFormation_Terrain::Tick(_float fTimeDelta)
{

}

void CFormation_Terrain::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CFormation_Terrain::Render()
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

HRESULT CFormation_Terrain::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom[TYPE_DIFFUSE])))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFormation_Terrain::SetUp_ShaderResource()
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
	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_SourDiffTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->Set_ShaderResourceView(m_pShaderCom, "g_DestDiffTexture", 1)))
		return E_FAIL;
	/*if (FAILED(m_pTextureCom[TYPE_FILTER]->Set_ShaderResourceView(m_pShaderCom, "g_FilterTexture", 0)))
	return E_FAIL;*/


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


void CFormation_Terrain::OnDisable()
{
}

void CFormation_Terrain::OnEnable()
{
}

CFormation_Terrain * CFormation_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFormation_Terrain*		pInstance = new CFormation_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFormation_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFormation_Terrain::Clone(void * pArg)
{
	CFormation_Terrain*		pInstance = new CFormation_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFormation_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFormation_Terrain::Free()
{
	__super::Free();

	for (auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
