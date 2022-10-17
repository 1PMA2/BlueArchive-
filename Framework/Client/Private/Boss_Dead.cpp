#include "stdafx.h"
#include "..\Public\Boss_Dead.h"

#include "GameInstance.h"
#include "Monster_State.h"
#include "Sensei.h"
#include "Student.h"

CBoss_Dead::CBoss_Dead(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBoss_Dead::CBoss_Dead(const CBoss_Dead & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBoss_Dead::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Dead::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_matrix WorldMatrix;

	if (nullptr != pArg)
		memcpy(&WorldMatrix, pArg, sizeof(_matrix));


	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, WorldMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, WorldMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, WorldMatrix.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, WorldMatrix.r[3]);


	m_pModelCom->Set_CurrentAnimation(10);

	CSensei* pSensei = GET_SENSEI;

	pSensei->Victory();

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_End"), TEXT("Prototype_GameObject_VictoryBg"));

	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_End"), TEXT("Prototype_GameObject_Victory"));

	return S_OK;
}

void CBoss_Dead::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);
}

void CBoss_Dead::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);


	if (m_pModelCom->Get_isFinished())
	{
		DELETE(this);
	}

}

HRESULT CBoss_Dead::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;




	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		return E_FAIL;*/

		m_pModelCom->Render(i, m_pShaderCom, 0, "g_Bones");
	}

	return S_OK;
}

void CBoss_Dead::OnDisable()
{
}

void CBoss_Dead::OnEnable()
{
}

HRESULT CBoss_Dead::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TankHihumi"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Dead::SetUp_ShaderResource()
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


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CBoss_Dead * CBoss_Dead::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_Dead*		pInstance = new CBoss_Dead(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBoss_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBoss_Dead::Clone(void * pArg)
{
	CBoss_Dead*		pInstance = new CBoss_Dead(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBoss_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Dead::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);

}