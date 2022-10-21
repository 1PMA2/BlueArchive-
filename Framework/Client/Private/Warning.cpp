#include "stdafx.h"
#include "..\Public\Warning.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Sensei.h"
#include "Warning_Bar.h"

CWarning::CWarning(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CWarning::CWarning(const CWarning & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWarning::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWarning::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	m_fSizeX = 480.f;
	m_fSizeY = 80.f;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(+ 0.f,  0.f, 0.f, 1.f));
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_int iDir = 1;
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_W"), TEXT("Prototype_GameObject_WarningBar"), &iDir);

	iDir = -1;
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_W"), TEXT("Prototype_GameObject_WarningBar"), &iDir);

	m_pTop = (CWarning_Bar*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_W"), 0);
	m_pBottom = (CWarning_Bar*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_W"), 1);

	return S_OK;
}

void CWarning::Tick(_float fTimeDelta)
{
	m_fCos += fTimeDelta * 2;
	m_fFade = (0.4f * fabs(cos(m_fCos))) + 0.4f;


	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));

	Deletethis(fTimeDelta);
	
}

void CWarning::LateTick(_float fTimeDelta)
{


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);


	/*if (4.f < m_fCos)
		DELETE(this);*/
}

HRESULT CWarning::Render()
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

HRESULT CWarning::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Warning"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWarning::SetUp_ShaderResource()
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
	if (FAILED(m_pShaderCom->Set_RawValue("g_UVx", &m_fUVx, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Warning", &m_iWarning, sizeof(_uint))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iWarning)))
		return E_FAIL;


	return S_OK;
}

void CWarning::Deletethis(_float fTimeDelta)
{
	if (4.f < m_fCos)
	{
		m_fSizeY -= 800.f * fTimeDelta;

		if (5.f > m_fSizeY)
		{
			DELETE(m_pTop);
			DELETE(m_pBottom);
			DELETE(this);
		}
	}
}

void CWarning::OnDisable()
{
}

void CWarning::OnEnable()
{
}

CWarning * CWarning::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CWarning*		pInstance = new CWarning(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWarning");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWarning::Clone(void * pArg)
{
	CWarning*		pInstance = new CWarning(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWarning");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWarning::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
