#include "stdafx.h"
#include "..\Public\Boss_HP.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Sensei.h"
#include "Camera_Ex.h"
#include "Monster.h"

CBoss_HP::CBoss_HP(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBoss_HP::CBoss_HP(const CBoss_HP & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBoss_HP::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_HP::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 500.f;
	m_fSizeY = 30.f;
	m_fLocationX = 0.f;
	m_fLocationY = 50.f;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	if (nullptr != pArg)
		memcpy(&m_pOwner, pArg, sizeof(CMonster*));

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, m_fY - m_fLocationY, 0.f, 1.f));

	m_fRatio = m_fSizeX / 5000;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CSensei* pSensei = CSensei::Get_Instance();

	return S_OK;
}

void CBoss_HP::Tick(_float fTimeDelta)
{
	m_iHp = m_pOwner->Get_MonsterHp();

	m_fSizeX = m_iHp * 0.1;

	m_fRatio = m_fSizeX / 500.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0, m_fY - m_fLocationY, 0.f, 1.f));
}

void CBoss_HP::LateTick(_float fTimeDelta)
{
	CSensei* pSensei = GET_SENSEI;

	if (!pSensei->Get_SenseiInfo().bEx)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}

	if (pSensei->Get_EndScene())
	{
		DELETE(this);
	}
}

HRESULT CBoss_HP::Render()
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

HRESULT CBoss_HP::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Hp"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_HP::SetUp_ShaderResource()
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
	if (FAILED(m_pShaderCom->Set_RawValue("g_Cost", &m_fRatio, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	return S_OK;
}

void CBoss_HP::OnDisable()
{
}

void CBoss_HP::OnEnable()
{
}

CBoss_HP * CBoss_HP::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_HP*		pInstance = new CBoss_HP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBoss_HP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBoss_HP::Clone(void * pArg)
{
	CBoss_HP*		pInstance = new CBoss_HP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBoss_HP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_HP::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}