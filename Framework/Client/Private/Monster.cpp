#include "stdafx.h"
#include "..\Public\Monster.h"

#include "GameInstance.h"
#include "Monster_State.h"
#include "M_Landing.h"
#include "Sensei.h"
#include "Student.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{
}

CStudent * CMonster::FoundStudent()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();

	_uint iStudentCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Student"));

	_vector vTranslation = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	if (0 >= iStudentCount)
	{
		return nullptr;
	}

	m_Students.clear();

	for (_uint i = 0; i < iStudentCount; ++i)
	{
		CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student"), i);

		CTransform* pStudentTransform = (CTransform*)pStudent->Get_Component(TEXT("Com_Transform"));

		_vector vStudentTranslation = pStudentTransform->Get_State(CTransform::STATE_TRANSLATION);

		_float fLength = XMVectorGetX(XMVector3Length(vStudentTranslation - vTranslation)); //학생과 모든 몬스터 사이의 거리 

		if (m_tMonsterInfo.fRange > fLength) //레이어의 몬스터 검사 후 범위 내 학생 
		{
			m_Students.push_back(pStudent);
		}
	}
	if (0 >= m_Students.size())
		return nullptr;

	m_fMin = 9999.f;

	for (_uint i = 0; i < m_Students.size(); ++i)
	{
		CTransform* pStudentTransform = (CTransform*)m_Students.at(i)->Get_Component(TEXT("Com_Transform"));

		_vector vMonsterTranslation = pStudentTransform->Get_State(CTransform::STATE_TRANSLATION);

		_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vTranslation));

		if (m_fMin > fLength)
		{
			m_fMin = fLength;
			m_pTargetStudent = m_Students.at(i);
		}
	}

	return m_pTargetStudent;
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	m_tMonsterInfo.iAtk = 10;
	m_tMonsterInfo.iHp = 200;
	m_tMonsterInfo.fRange = 4.f;
	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_vector vTranslation;

	if (nullptr != pArg)
		memcpy(&vTranslation, pArg, sizeof(_vector));


	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vTranslation);

	m_pState = CM_Landing::Create(this);

	return S_OK;
}

void CMonster::Tick(_float fTimeDelta)
{

	if (CMonster_State*	pNewState = m_pState->Loop(fTimeDelta))
	{
		m_pState->Exit();
		m_pState = pNewState;
		m_pState->Enter();
	}


	if (m_bFear)
	{
		m_fFearAcc += fTimeDelta;

		if (1.f < m_fFearAcc)
		{
			m_bFear = false;
			m_fFearAcc = 0.f;
		}
	}


	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());

	SelectMonster();

	
}

void CMonster::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	
	DeleteMonster();
}

HRESULT CMonster::Render()
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
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CMonster::OnDisable()
{
}

void CMonster::OnEnable()
{
}

HRESULT CMonster::SetUp_Components()
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

	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Droid"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::SetUp_ShaderResource()
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

void CMonster::SelectMonster()
{
	CSensei* pSensei = GET_SENSEI;


	if (m_pSphereCom->CollisionRay())
	{
		if (pSensei->Get_ExReady())
		{
			if (KEY(LBUTTON, TAP))
			{
				pSensei->Ex_Lockon(this);
			}
		}
	}
	pSensei->Get_LockonVector();
	
}

void CMonster::DeleteMonster()
{
	if (0 >= m_tMonsterInfo.iHp)
	{
		CSensei* pSensei = GET_SENSEI;

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();

		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Dead"), TEXT("Prototype_GameObject_Dead"), &WorldMatrix);

		if(this == pSensei->Get_LockonMonster())
			pSensei->Ex_Lockon(nullptr);

		DELETE(this);
	}
}

CMonster * CMonster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMonster*		pInstance = new CMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonster::Clone(void * pArg)
{
	CMonster*		pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	if (m_pState)
	{
		m_pState->Destroy_Instance();
		m_pState = nullptr;
	}

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);


}