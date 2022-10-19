#include "stdafx.h"
#include "..\Public\Formation_Window.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Formaton_Btn.h"
#include "Sensei.h"

CFormation_Window::CFormation_Window(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CFormation_Window::CFormation_Window(const CFormation_Window & rhs)
	: CGameObject(rhs)
{
}

HRESULT CFormation_Window::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFormation_Window::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	m_fSizeX = 1050.f;
	m_fSizeY = 1300.f;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, m_fY - 1300, 0.f, 1.f));
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_pBtn = (CFormaton_Btn*)pGameInstance->Get_GameObject(LEVEL_FORMATION, TEXT("Layer_Formation_BackGround"), 3);

	if (nullptr == m_pBtn)
		return E_FAIL;

	CSensei* pSensei = CSensei::Get_Instance();

	/* For.Player */
	for (int i = 0; i < pSensei->Get_StudentNum(); ++i)
	{
		STUDENT eStudent = pSensei->Get_StudentIndex(i)->Get_StudentInfo().eStudent;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_FORMATION, TEXT("Layer_Portrait"), TEXT("Prototype_GameObject_Portrait"), &eStudent))) //뽑은 모든 학생 넘김 이름정보만 가지고있음
			return E_FAIL;

	}



	return S_OK;
}

void CFormation_Window::Tick(_float fTimeDelta)
{
	OpenWindow(fTimeDelta);

}

void CFormation_Window::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CFormation_Window::Render()
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

HRESULT CFormation_Window::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_FORMATION, TEXT("Prototype_Component_Texture_FormationWindow"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFormation_Window::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

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

void CFormation_Window::OnDisable()
{
}

void CFormation_Window::OnEnable()
{
}

void CFormation_Window::OpenWindow(_float fTimeDelta)
{
	if (m_pBtn->Get_Openwindow())
	{
		_vector vOrigin = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTarget = XMVectorSet(0.f, m_fY - 700, 0.f, 1.f);

		_vector vLerp = XMVectorLerp(vOrigin, vTarget, fTimeDelta * 5.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vLerp);

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		//ENABLE(pGameInstance->Get_GameObject(LEVEL_FORMATION, TEXT("Layer_Formation_Student")));
	}
	else
	{
		_vector vOrigin = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTarget = XMVectorSet(0.f, m_fY - 1400, 0.f, 1.f);

		_vector vLerp = XMVectorLerp(vOrigin, vTarget, fTimeDelta * 5.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vLerp);
	}

}

CFormation_Window * CFormation_Window::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFormation_Window*		pInstance = new CFormation_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFormation_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFormation_Window::Clone(void * pArg)
{
	CFormation_Window*		pInstance = new CFormation_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFormation_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFormation_Window::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}