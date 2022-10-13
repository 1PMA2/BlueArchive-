#include "stdafx.h"
#include "..\Public\Student.h"

#include "GameInstance.h"
#include "State.h"
#include "Run.h"
#include "Sensei.h"
#include "Ex_Cutin.h"
#include "Formation_Idle.h"
#include "Aru.h"
#include "Monster.h"
#include "ForkLift.h"

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

CMonster* CStudent::FoundMonster()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();

	_uint iMonsterCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

	_vector vTranslation = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	if (0 >= iMonsterCount)
	{
		return nullptr;
	}

	m_Monsters.clear();

	for (_uint i = 0; i < iMonsterCount; ++i)
	{
		CMonster* pMonster = (CMonster*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), i);

		CTransform* pMonsterTransform = (CTransform*)pMonster->Get_Component(TEXT("Com_Transform"));

		_vector vMonsterTranslation = pMonsterTransform->Get_State(CTransform::STATE_TRANSLATION);

		_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vTranslation)); //학생과 모든 몬스터 사이의 거리 

		if (m_tStudentInfo.fRange > fLength) //레이어의 몬스터 검사 후 범위 내 몬스터 
		{
			m_Monsters.push_back(pMonster);
		}
	}
	if (0 >= m_Monsters.size())
		return nullptr;

	m_fMin = 9999.f;

	for (_uint i = 0; i < m_Monsters.size(); ++i)
	{
		_vector vMonsterTranslation = m_Monsters.at(i)->Get_MonsterTranslation();

		_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vTranslation));

		if (m_fMin > fLength)
		{
			m_fMin = fLength;
			m_pTargetMonster = m_Monsters.at(i);
		}
	}

	return m_pTargetMonster;
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

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(0.52f, 1.f, 0.52f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(0.4f, 0.4f, 0.4f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
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


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CStudent::FormationLevel_Collision(_float fTimeDelta)
{

	

	return S_OK;
}

HRESULT CStudent::GamePlayLevel_Collision(_float fTimeDelta)
{
	m_bColled = false;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	for (_uint i = 0; i < pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Cover")); ++i)
	{
		CForkLift* pCover = (CForkLift*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cover"), i);
		CCollider* pSilde = (CCollider*)pCover->Get_Component(TEXT("Com_SlideAABB"));

		if (m_pSphereCom->Collision(pSilde))
		{
			m_bColled = true;

			CTransform* m_pSlideTransform = (CTransform*)pCover->Get_Component(TEXT("Com_Transform"));
			_vector vTranslation = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
			_vector vSlide = m_pSlideTransform->Get_State(CTransform::STATE_TRANSLATION);
			_vector vLook = vTranslation - vSlide;

			_float fDir = XMVectorGetX(vLook);
			if (-0.2f <= fDir)
				m_pTransformCom->LookAtLerp(XMVectorSet(5.f, 0.f, XMVectorGetZ(vSlide) - 1.f, 1.f), 7.f, fTimeDelta);
			else
				m_pTransformCom->LookAtLerp(XMVectorSet(-5.f, 0.f, XMVectorGetZ(vSlide) - 1.f, 1.f), 7.f, fTimeDelta);
			break;
		}

	}

	ToStudentCollision(fTimeDelta);

	return S_OK;
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

			for (_uint i = 0; i < pSensei->Get_FormationInfoSize(); ++i)
			{
				if (pSensei->Get_FormationInfo(i).eStudent == m_tStudentInfo.eStudent)
				{
					m_tStudentInfo.eFormation = pSensei->Get_FormationInfo(i).eFormation;
					break;
				}
			}

			FORMATION eFormation = m_tStudentInfo.eFormation;

			_vector m_vPreTranslation;

			switch (eFormation)
			{
			case FORMATION_FIRST:
				m_vPreTranslation = XMVectorSet(-1.5f, 0.f, 0.f, 1.f);
				break;
			case FORMATION_SECOND:
				m_vPreTranslation = XMVectorSet(-0.5f, 0.f, 0.f, 1.f);
				break;
			case FORMATION_THIRD:
				m_vPreTranslation = XMVectorSet(0.5f, 0.f, 0.f, 1.f);
				break;
			case FORMATION_FOURTH:
				m_vPreTranslation = XMVectorSet(1.5f, 0.f, 0.f, 1.f);
				break;
			}

			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_vPreTranslation);
			break;
		}

	}
	else
	{
		switch (m_tStudentInfo.eFormation)
		{
		case FORMATION_FIRST:
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, -20.f, 0.f, 1.f));
			break;
		case FORMATION_SECOND:
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, -40.f, 0.f, 1.f));
			break;
		case FORMATION_THIRD:
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, -60.f, 0.f, 1.f));
			break;
		}
	}
	RELEASE_INSTANCE(CSensei);
}

void CStudent::ToStudentCollision(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (!m_bOther)
	{
		for (_uint i = 0; i < pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Student")); ++i)
		{
			CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student"), i);

			if (pStudent == this)
				continue;

			CCollider* pOther = (CCollider*)pStudent->Get_Component(TEXT("Com_AABB"));

			if (m_pAABBCom->Collision(pOther))
			{
				m_bOther = true;
				break;
			}
		}
	}

	else
	{
		m_fTimeAcc += fTimeDelta;

		if (0.5f < m_fTimeAcc)
		{
			m_bOther = false;
			m_fTimeAcc = 0.f;
		}
	}


}

void CStudent::Free()
{
	__super::Free();

	if (m_pState)
	{
		m_pState->Destroy_Instance();
		m_pState = nullptr;
	}

	m_Monsters.clear();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
}

