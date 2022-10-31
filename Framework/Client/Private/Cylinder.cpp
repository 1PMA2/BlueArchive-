#include "stdafx.h"
#include "..\Public\Cylinder.h"

#include "GameInstance.h"
#include "Student.h"
#include "Monster.h"
#include "HIerarchyNode.h"
#include "Collider.h"

CCylinder::CCylinder(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CCylinder::CCylinder(const CCylinder & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCylinder::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCylinder::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_pOwner, pArg, sizeof(CMonster*));

	InitLook();
	
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_vT);
	return S_OK;
}

void CCylinder::Tick(_float fTimeDelta)
{
	if (nullptr == m_pModelCom)
		return;

	m_fTick += fTimeDelta * 10.f;

	m_fSize = Lerp(m_fSize, 8.f, fTimeDelta);

	m_pTransformCom->Set_Scaled(_float3(8.f - m_fSize, 8.f - m_fSize, 25.f));

}

void CCylinder::LateTick(_float fTimeDelta)
{

	if (m_fTick > 20.f)
		DELETE(this);


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CCylinder::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pModelCom->Render(0, m_pShaderCom, 2);

	return S_OK;
}

HRESULT CCylinder::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Wave"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Cylinder0"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCylinder::SetUp_ShaderResource()
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
	if (FAILED(m_pShaderCom->Set_RawValue("g_UVx", &m_fTick, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CCylinder::InitLook()
{
	CTransform* pMuzzle = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel*		pTargetModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	m_pBonePtr = pTargetModel->Find_HierarcyNode("Fire_01");

	if (nullptr == m_pBonePtr)
		return E_FAIL;

	_matrix		ParentMatrix = m_pBonePtr->Get_OffsetMatrix() * m_pBonePtr->Get_CombinedMatrix() * m_pBonePtr->Get_TransformMatrix();
	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, ParentMatrix * pMuzzle->Get_WorldMatrix());

	_vector vTranslation = XMLoadFloat4x4(&m_WorldMatrix).r[CTransform::STATE_TRANSLATION];
	_vector		vLook = pMuzzle->Get_WorldMatrix().r[2];

	m_pTransformCom->Rotation(vLook, 0.5f);

	vTranslation -= XMVector3Normalize(vLook) * 4.5f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vTranslation);

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, pMuzzle->Get_WorldMatrix().r[2]);

	m_pTransformCom->Set_Scaled(_float3(8.f, 8.f, 25.f));

	return S_OK;
}

_float CCylinder::Lerp(_float fStart, _float fEnd, _float fTime)
{
	fTime = fTime * 5.f;

	_float fLerp = (1 - fTime) * fStart + (fTime * fEnd);

	return fLerp;
}

void CCylinder::OnDisable()
{
}

void CCylinder::OnEnable()
{
}

CCylinder * CCylinder::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCylinder*		pInstance = new CCylinder(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCylinder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCylinder::Clone(void * pArg)
{
	CCylinder*		pInstance = new CCylinder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCylinder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCylinder::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);

}
