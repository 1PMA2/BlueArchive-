#include "stdafx.h"
#include "..\Public\Fade_InOut.h"

#include "GameInstance.h"
#include "Sensei.h"

CFade_InOut::CFade_InOut(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CFade_InOut::CFade_InOut(const CFade_InOut & rhs)
	: CGameObject(rhs)
{
}

HRESULT CFade_InOut::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFade_InOut::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_fSizeX = 1600.f;
	m_fSizeY = 1124.f;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	if (nullptr != pArg)
		memcpy(&m_iImgNum, pArg, sizeof(_int));

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

	return S_OK;
}

void CFade_InOut::Tick(_float fTimeDelta)
{
	if (1.f == m_fFade)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CSensei* pSensei = GET_SENSEI;


		_int iImgNum = 2;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHASCENE, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_BG"), &iImgNum)))
			return;

		iImgNum = pSensei->Get_Student(pSensei->Get_NewStudent())->Get_StudentInfo().eStudent;

		/* For.Player */
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHASCENE, TEXT("Layer_Student"), TEXT("Prototype_GameObject_StudentImg"), &iImgNum)))
			return;

		Safe_Release(pGameInstance);
	}


	m_fFade += 0.02f * m_fOdd;
	
	if(1.f < m_fFade)
	{
		m_fFade = 1.f;
		m_fOdd *= -1.f;
	}
	else if (0.f > m_fFade)
	{
		m_fFade = 0.f;
		m_fOdd *= 1.f;
		this->Set_Enable(false);
	}

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));

}

void CFade_InOut::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	if (m_bIsFinished)
		this->Set_Enable(false);
}

HRESULT CFade_InOut::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	/*m_fSizeX = (_float)m_pTextureCom->Get_TextureSize(m_iImgNum).Width;
	m_fSizeY = (_float)m_pTextureCom->Get_TextureSize(m_iImgNum).Height;*/

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));


	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CFade_InOut::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFade_InOut::SetUp_ShaderResource()
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

void CFade_InOut::OnDisable()
{

}

void CFade_InOut::OnEnable()
{
	m_bIsFinished = false;
	m_fFade = 1.f;
}

CFade_InOut * CFade_InOut::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFade_InOut*		pInstance = new CFade_InOut(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFade_InOut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFade_InOut::Clone(void * pArg)
{
	CFade_InOut*		pInstance = new CFade_InOut(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFade_InOut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFade_InOut::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
