#include "stdafx.h"
#include "..\Public\Student.h"

#include "GameInstance.h"
#include "State.h"
#include "Run.h"
#include "Sensei.h"
#include "Ex_Cutin.h"
#include "Formation_Idle.h"
#include "Aru.h"

CStudent::CStudent(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
	
}

CStudent::CStudent(const CStudent & rhs)
	: CGameObject(rhs)
	, m_tStudentInfo(rhs.m_tStudentInfo)
	, m_pState(nullptr)
{
}

HRESULT CStudent::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	InitializeStudentState();

	return S_OK;
}

void CStudent::Tick(_float fTimeDelta)
{

	if (false == m_tStudentInfo.bExModel) //스킬모델이 아닐시 재생 안함
	{
		m_pModelCom->Play_Animation(fTimeDelta);
	}

	if (CState*	pNewState = m_pState->Loop(fTimeDelta))
	{
		m_pState->Exit();
		m_pState = pNewState;
		m_pState->Enter();
	}
	
	_matrix NoneScaleWorldMatrix;
	NoneScaleWorldMatrix.r[0] = XMVector3Normalize(m_pTransformCom->Get_WorldMatrix().r[0]);
	NoneScaleWorldMatrix.r[1] = XMVector3Normalize(m_pTransformCom->Get_WorldMatrix().r[1]);
	NoneScaleWorldMatrix.r[2] = XMVector3Normalize(m_pTransformCom->Get_WorldMatrix().r[2]);
	NoneScaleWorldMatrix.r[3] = m_pTransformCom->Get_WorldMatrix().r[3];

	m_pAABBCom->Update(NoneScaleWorldMatrix);
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CStudent::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);


}

HRESULT CStudent::Render()
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

#ifdef _DEBUG
	m_pAABBCom->Render();
	m_pOBBCom->Render();
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CStudent::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStudent::SetUp_ShaderResource()
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


	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);

	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fRange", &pLightDesc->fRange, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CStudent::FormationLevel_Collision()
{
	if (m_pAABBCom->CollisionRay())
	{
		if (KEY(LBUTTON, TAP))
		{
			m_bPicked = true;
			m_pPickedCom = m_pAABBCom;
		}

	}
	if (KEY(LBUTTON, AWAY))
	{
		m_bPicked = false;
		m_pPickedCom = nullptr;
	}

	CSensei* pSensei = GET_SENSEI;

	if (nullptr != m_pPickedCom)
	{
		for (_uint i = 0; i < pSensei->Get_FormationStudentsNum(); ++i)
		{
			CStudent* pTargetStudent = pSensei->Get_FormationStudents(i);
			if (m_tStudentInfo.iIndex != pTargetStudent->Get_StudentInfo().iIndex)
			{
				if (m_pPickedCom->Collision(pTargetStudent->m_pAABBCom))
				{
					pTargetStudent->m_pTransformCom->Set_Scaled({ 0.8f, 0.8f, 0.8f });
				}
				else
					pTargetStudent->m_pTransformCom->Set_Scaled({ 1.f, 1.f, 1.f });
			}
		}
	}


	return S_OK;
}

HRESULT CStudent::GamePlayLevel_Collision()
{
	return E_NOTIMPL;
}

void CStudent::InitializeStudentState()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CSensei* pSensei = GET_INSTANCE(CSensei);

	LEVEL eLEVEL = pSensei->Get_CurrentLevel();

	if (false == m_tStudentInfo.bExModel)
	{
		
		switch (eLEVEL)
		{
		case LEVEL_FORMATION:
			pSensei->Set_FormationStudents(this);
			m_pState = CFormation_Idle::Create(this);
			break;
		case LEVEL_GAMEPLAY:
			m_pState = CRun::Create(this);
			break;
		}

	}
	else
	{
		m_pState = CEx_Cutin::Create(this);

		switch (m_tStudentInfo.eFormation)
		{
		case FORMATION_FIRST:
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, -20.f, -5.f, 1.f));
			break;
		case FORMATION_SECOND:
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
			break;
		}
	}
	RELEASE_INSTANCE(CSensei);
}

void CStudent::Free()
{
	__super::Free();

	if (m_pState)
	{
		m_pState->Destroy_Instance();
		m_pState = nullptr;
	}

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pSphereCom_Obstacle);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
}

