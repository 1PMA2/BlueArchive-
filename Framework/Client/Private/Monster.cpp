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
		m_pTargetStudent = nullptr;

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
			if(false == pStudent->Is_Retire())
				m_Students.push_back(pStudent);
		}
	}
	if (0 >= m_Students.size())
	{
		m_pTargetStudent = nullptr;

		return nullptr;
	}

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
			m_vTargetTranslation = m_pTargetStudent->Get_StudentTranslation();
		}
	}


	return m_pTargetStudent;
}

void CMonster::Set_MinusHp(_int iAtk)
{
	m_bDamaged = true;

	_uint iDamage = (_int)(iAtk * frandom(0.9, 1.1));
	m_tMonsterInfo.iHp -= iDamage;
	Damage(iDamage);
}

CStudent * CMonster::Get_RandomStudent()
{

	if (0 < m_Students.size())
	{
		_uint iRand = random(0, (_int)m_Students.size() - 1);
		m_pRandomStudent = m_Students.at(iRand);
		m_vRandomTarget = m_pRandomStudent->Get_StudentTranslation();
		return m_pRandomStudent;
	}

	else
		return nullptr;

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(&m_tTransformDesc)))
		return E_FAIL;


	return S_OK;
}

void CMonster::Tick(_float fTimeDelta)
{
	FoundStudent();

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


	if (m_bDamaged)
	{
		m_fDamagedAcc += fTimeDelta;

		if (0.05f < m_fDamagedAcc)
		{
			m_bDamaged = false;
			m_fDamagedAcc = 0.f;
		}
	}


	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());

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
	m_pAABBCom->Render();
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
	if (FAILED(m_pShaderCom->Set_RawValue("g_Damaged", &m_bDamaged, sizeof(_bool))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CMonster::SelectMonster()
{
	CSensei* pSensei = GET_SENSEI;

	if (this == pSensei->Get_LockonMonster())
		pSensei->Get_LockonVector();

	if (m_pAABBCom->CollisionRay())
	{
		if (pSensei->Get_ExReady())
		{
			if (KEY(LBUTTON, TAP))
			{
				pSensei->Ex_Lockon(this);
			}
		}
	}
	
}

void CMonster::DeleteMonster()
{
	CSensei* pSensei = GET_SENSEI;

	if (pSensei->Get_EndScene())
	{
		DELETE(this);
	}



	if ((0 >= m_tMonsterInfo.iHp))
	{
		

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();

		switch (m_tMonsterInfo.eMonster)
		{
		case DROID:
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Dead"), TEXT("Prototype_GameObject_Dead"), &WorldMatrix);
			DELETE(this);
			break;

		case BOSS:
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Dead"), TEXT("Prototype_GameObject_BossDead"), &WorldMatrix);
			DISABLE(this);
			break;

		}

		if(this == pSensei->Get_LockonMonster())
			pSensei->Ex_Lockon(nullptr);

		
	}
}

void CMonster::Damage(_uint iDamage)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	DAMAGE tD = {};

	tD.vTranslation = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	tD.iNum = iDamage % 10;
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Damage"), &tD);

	tD.iNum = ((iDamage / 10) % 10);
	if ((10 <= iDamage) && (0 <= tD.iNum))
	{
		tD.vTranslation = XMVectorSet(XMVectorGetX(tD.vTranslation), XMVectorGetY(tD.vTranslation) + 0.05f, XMVectorGetZ(tD.vTranslation) - 0.16f, 1.f);
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Damage"), &tD);
	}

	tD.iNum = iDamage / 100;
	if (0 < tD.iNum)
	{
		tD.vTranslation = XMVectorSet(XMVectorGetX(tD.vTranslation), XMVectorGetY(tD.vTranslation) + 0.05f, XMVectorGetZ(tD.vTranslation) - 0.16f, 1.f);
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Damage"), &tD);
	}
}

//CMonster * CMonster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
//{
//	CMonster*		pInstance = new CMonster(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype()))
//	{
//		MSG_BOX("Failed to Created : CMonster");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//CGameObject * CMonster::Clone(void * pArg)
//{
//	CMonster*		pInstance = new CMonster(*this);
//
//	if (FAILED(pInstance->Initialize(pArg)))
//	{
//		MSG_BOX("Failed to Cloned : CMonster");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

void CMonster::Free()
{
	__super::Free();

	if (m_pState)
	{
		m_pState->Destroy_Instance();
		m_pState = nullptr;
	}

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);


}