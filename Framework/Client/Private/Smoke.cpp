#include "stdafx.h"
#include "..\Public\Smoke.h"

#include "GameInstance.h"
#include "Student.h"
#include "HIerarchyNode.h"

CSmoke::CSmoke(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CSmoke::CSmoke(const CSmoke & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSmoke::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSmoke::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 0.2f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_pOwner, pArg, sizeof(CStudent*));

	InitLook();

	return S_OK;
}

void CSmoke::Tick(_float fTimeDelta)
{
	if (nullptr == m_pVIBufferCom)
		return;

	m_pTransformCom->Go_Straight(fTimeDelta);

	m_fFrame += 20.f * fTimeDelta;


	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	_matrix vInv = pGameInstance->Get_Inv();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vInv.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vInv.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vInv.r[2]);

	m_pTransformCom->Set_Scaled(_float3(0.6f, 0.6f, 0.6f));

	Safe_Release(pGameInstance);

	if (m_fFrame > 9.f)
	{
		DELETE(this);
	}
}

void CSmoke::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CSmoke::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(3);

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CSmoke::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Smoke"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSmoke::SetUp_ShaderResource()
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
	if (FAILED(m_pShaderCom->Set_RawValue("g_Frame", &m_fFrame, sizeof(_int))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CSmoke::InitLook()
{
	CTransform* pMuzzle = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));


	CModel*		pTargetModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	m_pBonePtr = pTargetModel->Find_HierarcyNode("fire_01");

	if (nullptr == m_pBonePtr)
		return E_FAIL;

	_matrix		ParentMatrix = m_pBonePtr->Get_OffsetMatrix() * m_pBonePtr->Get_CombinedMatrix() * m_pBonePtr->Get_TransformMatrix();
	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, ParentMatrix * pMuzzle->Get_WorldMatrix());

	_vector vTranslation = XMLoadFloat4x4(&m_WorldMatrix).r[CTransform::STATE_TRANSLATION];
	_vector		vLook = pMuzzle->Get_WorldMatrix().r[2];

	vTranslation += XMVector3Normalize(vLook) * 0.5f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vTranslation);

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);


	return S_OK;
}

void CSmoke::OnDisable()
{
}

void CSmoke::OnEnable()
{
}

CSmoke * CSmoke::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSmoke*		pInstance = new CSmoke(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSmoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSmoke::Clone(void * pArg)
{
	CSmoke*		pInstance = new CSmoke(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSmoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSmoke::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

}

