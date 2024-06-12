#include "stdafx.h"
#include "..\Public\Sky_Ex.h"

#include "GameInstance.h"
#include "Sensei.h"
#include "Student.h"
CSky_Ex::CSky_Ex(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CSky_Ex::CSky_Ex(const CSky_Ex & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSky_Ex::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSky_Ex::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CSky_Ex::Tick(_float fTimeDelta)
{
	CSensei* pSensei = GET_SENSEI;
	
	CStudent* pStudent = pSensei->Get_ExStudent();

	if (pStudent)
		m_iSkyNum = pStudent->Get_StudentInfo().eStudent;
	else
		m_iSkyNum = 0;
}

void CSky_Ex::LateTick(_float fTimeDelta)
{
	CSensei* pSensei = GET_SENSEI;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_vector vCampos = XMLoadFloat4(&pGameInstance->Get_CamPosition());

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vCampos);

	RELEASE_INSTANCE(CGameInstance);

	if (pSensei->Get_SenseiInfo().bEx)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CSky_Ex::Render()
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

HRESULT CSky_Ex::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Ex_Sky"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSky_Ex::SetUp_ShaderResource()
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
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iSkyNum)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CSky_Ex::OnDisable()
{
}

void CSky_Ex::OnEnable()
{
}

CSky_Ex * CSky_Ex::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSky_Ex*		pInstance = new CSky_Ex(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSky_Ex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSky_Ex::Clone(void * pArg)
{
	CSky_Ex*		pInstance = new CSky_Ex(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSky_Ex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSky_Ex::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
