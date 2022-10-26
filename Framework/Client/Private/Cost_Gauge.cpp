#include "stdafx.h"
#include "..\Public\Cost_Gauge.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Sensei.h"
#include "Camera_Ex.h"

CCost_Gauge::CCost_Gauge(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CCost_Gauge::CCost_Gauge(const CCost_Gauge & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCost_Gauge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCost_Gauge::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fMaxSize = 315.f;
	m_fSizeX = 315.f;
	m_fSizeY = 15.f;
	m_fLocationX = 900.f;
	m_fLocationY = 705.f;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

	CSensei* pSensei = GET_SENSEI;

	pSensei->ReSet_Cost();

	return S_OK;
}

void CCost_Gauge::Tick(_float fTimeDelta)
{
	
	CSensei* pSensei = GET_SENSEI;

	pSensei->Tick_Cost(fTimeDelta);

	m_fCost = pSensei->Get_Cost() * 0.1f;

	m_fSizeX = m_fMaxSize * m_fCost;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-m_fX + m_fLocationX + (m_fSizeX * 0.5f), m_fY - m_fLocationY, 0.f, 1.f));  //임시 > 부모만들어서 컷신일대 렌더하지말것
}

void CCost_Gauge::LateTick(_float fTimeDelta)
{
	CSensei* pSensei = GET_SENSEI;

	if (!pSensei->Get_SenseiInfo().bEx)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CCost_Gauge::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CCost_Gauge::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_CostGauge"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCost_Gauge::SetUp_ShaderResource()
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
	if (FAILED(m_pShaderCom->Set_RawValue("g_Cost", &m_fCost, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Fade", &m_fFade, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 1)))
		return E_FAIL;


	return S_OK;
}

void CCost_Gauge::OnDisable()
{
}

void CCost_Gauge::OnEnable()
{
}

CCost_Gauge * CCost_Gauge::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCost_Gauge*		pInstance = new CCost_Gauge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCost_Gauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCost_Gauge::Clone(void * pArg)
{
	CCost_Gauge*		pInstance = new CCost_Gauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCost_Gauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCost_Gauge::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}