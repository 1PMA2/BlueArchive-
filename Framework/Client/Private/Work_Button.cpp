#include "stdafx.h"
#include "..\Public\Work_Button.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Sensei.h"

CWork_Button::CWork_Button(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
	
}

CWork_Button::CWork_Button(const CWork_Button & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWork_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWork_Button::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	m_fSizeX = 280.f;
	m_fSizeY = 234.f;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-m_fX + 1100.f, m_fY - 450.f, 0.f, 1.f));
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));

	return S_OK;
}

void CWork_Button::Tick(_float fTimeDelta)
{

	_float3 vPos;

	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	vPos.x += (g_iWinCX >> 1);
	vPos.y = -vPos.y + (g_iWinCY >> 1);

	_float3 vSize = _float3(m_fSizeX, m_fSizeY, 1.f);

	RECT rcButton;

	SetRect(&rcButton,
		(_int)(vPos.x - vSize.x * 0.5f),
		(_int)(vPos.y - vSize.y * 0.5f),
		(_int)(vPos.x + vSize.x * 0.5f),
		(_int)(vPos.y + vSize.y * 0.5f));

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rcButton, ptMouse))
	{
		if (KEY(LBUTTON, TAP))
		{
			m_pTransformCom->Set_Scaled(_float3(vSize.x * 1.1f, vSize.y * 1.1f, 0.f));
		}
		else if (KEY(LBUTTON, AWAY))
		{
			m_pTransformCom->Set_Scaled(_float3(vSize.x, vSize.y, 0.f));

			CSensei* pSensei = GET_SENSEI;

			pSensei->Set_OpenFormationLevel(true);
		}

	}
	else
		m_pTransformCom->Set_Scaled(_float3(vSize.x, vSize.y, 0.f));

}

void CWork_Button::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CWork_Button::Render()
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

HRESULT CWork_Button::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_WorkButton"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWork_Button::SetUp_ShaderResource()
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

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iImgNum)))
		return E_FAIL;


	return S_OK;
}

void CWork_Button::OnDisable()
{
}

void CWork_Button::OnEnable()
{
}

CWork_Button * CWork_Button::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CWork_Button*		pInstance = new CWork_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWork_Button::Clone(void * pArg)
{
	CWork_Button*		pInstance = new CWork_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWork_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWork_Button::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}