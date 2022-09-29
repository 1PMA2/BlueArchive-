#include "stdafx.h"
#include "..\Public\UI_RecruitButton.h"

#include "GameInstance.h"
#include "Sensei.h"

CUI_RecruitButton::CUI_RecruitButton(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CUI_RecruitButton::CUI_RecruitButton(const CUI_RecruitButton & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_RecruitButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_RecruitButton::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	m_fSizeX = 298.f;
	m_fSizeY = 103.f;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-m_fX + 1000.f, m_fY - 500.f, 0.f, 1.f));
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));

	return S_OK;
}

void CUI_RecruitButton::Tick(_float fTimeDelta)
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
			CSensei* pSensei = GET_SENSEI;
			
			pSensei->Set_OpenGachaScene(true);
		}
	}

}

void CUI_RecruitButton::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_RecruitButton::Render()
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

HRESULT CUI_RecruitButton::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GACHA, TEXT("Prototype_Component_Texture_Recruit"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_RecruitButton::SetUp_ShaderResource()
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

_bool CUI_RecruitButton::Random(_float fPercent)
{
	return false;
}

void CUI_RecruitButton::OnDisable()
{
}

void CUI_RecruitButton::OnEnable()
{
}

CUI_RecruitButton * CUI_RecruitButton::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_RecruitButton*		pInstance = new CUI_RecruitButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_RecruitButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_RecruitButton::Clone(void * pArg)
{
	CUI_RecruitButton*		pInstance = new CUI_RecruitButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_RecruitButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_RecruitButton::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
