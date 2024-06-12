#include "stdafx.h"
#include "..\Public\Workspace.h"
#include "GameInstance.h"

CWorkspace::CWorkspace(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CWorkspace::CWorkspace(const CWorkspace & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWorkspace::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWorkspace::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_iImgNum, pArg, sizeof(_int));

	m_fSizeX = 1280.f;
	m_fSizeY = 720.f;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	return S_OK;
}

void CWorkspace::Tick(_float fTimeDelta)
{
	m_fFrame += 20 * fTimeDelta;

	if (m_fFrame > 150.f)
		m_fFrame = 0.f;
}

void CWorkspace::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CWorkspace::Render()
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

HRESULT CWorkspace::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Workpage"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWorkspace::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
	return E_FAIL;*/
	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Fade", &m_fFade, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_fFrame)))
		return E_FAIL;


	return S_OK;
}

void CWorkspace::OnDisable()
{
}

void CWorkspace::OnEnable()
{
}

CWorkspace * CWorkspace::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CWorkspace*		pInstance = new CWorkspace(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWorkspace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWorkspace::Clone(void * pArg)
{
	CWorkspace*		pInstance = new CWorkspace(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWorkspace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWorkspace::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
