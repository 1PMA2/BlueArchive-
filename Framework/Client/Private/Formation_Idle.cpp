#include "stdafx.h"
#include "..\Public\Formation_Idle.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Formation_Pick.h"
#include "Sensei.h"
#include "Formaton_Btn.h"

#define FIRST_POS XMVectorSet(1.5f, 0.f, 0.f, 1.f)
#define SECOND_POS XMVectorSet(0.5f, 0.f, 0.f, 1.f)
#define THIRD_POS XMVectorSet(-0.5f, 0.f, 0.f, 1.f)
#define FOURTH_POS XMVectorSet(-1.5f, 0.f, 0.f, 1.f)
#define DEFAULT_POS XMVectorSet(-5.f, 0.f, 0.f, 1.f)
#define S_SIZE _float3(0.8f, 0.8f, 0.8f)


CFormation_Idle::CFormation_Idle(CStudent* pOwner)
	:CState(pOwner)
{
	Get_Btn();

	m_eAnim = ANIM_FORMATIONIDLE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case KAYOKO:
		pModel->Set_CurrentAnimation(0);
		break;
	case HARUKA:
		pModel->Set_CurrentAnimation(21);
		break;
	}

	CSensei* pSensei = GET_SENSEI;

	//if (0 >= pSensei->Get_FormationInfoSize())
	//{
	//	switch (m_pOwner->Get_StudentInfo().eFormation)
	//	{
	//	case FORMATION_FIRST:
	//		m_vPreTranslation = XMVectorSet(1.5f, 0.f, 0.f, 1.f);
	//		break;
	//	case FORMATION_SECOND:
	//		m_vPreTranslation = XMVectorSet(0.5f, 0.f, 0.f, 1.f);
	//		break;
	//	case FORMATION_THIRD:
	//		m_vPreTranslation = XMVectorSet(-0.5f, 0.f, 0.f, 1.f);
	//		break;
	//	case FORMATION_FOURTH:
	//		m_vPreTranslation = XMVectorSet(-1.5f, 0.f, 0.f, 1.f);
	//		break;
	//	case FORMATION_END:
	//		DISABLE(m_pOwner);
	//		break;
	//	}
	//}

	//else
	//{
		for (_uint i = 0; i < pSensei->Get_FormationInfoSize(); ++i)
		{
			if (pSensei->Get_FormationInfo(i).eStudent == m_pOwner->Get_StudentInfo().eStudent)
			{
				m_pOwner->Set_Formation(pSensei->Get_FormationInfo(i).eFormation);

				switch (m_pOwner->Get_StudentInfo().eFormation)
				{
				case FORMATION_FIRST:
					m_vPreTranslation = FIRST_POS;
					break;
				case FORMATION_SECOND:
					m_vPreTranslation = SECOND_POS;
					break;
				case FORMATION_THIRD:
					m_vPreTranslation = THIRD_POS;
					break;
				case FORMATION_FOURTH:
					m_vPreTranslation = FOURTH_POS;
					break;
				default:
					m_vPreTranslation = DEFAULT_POS;
					break;
				}
				
				break;
			}
		}
	//}
	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));
	pTransform->Set_State(CTransform::STATE_TRANSLATION, m_vPreTranslation);
}

void CFormation_Idle::Enter()
{
	
}

CState * CFormation_Idle::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CSensei* pSensei = GET_SENSEI;

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CCollider* pAABBcom = (CCollider*)m_pOwner->Get_Component(TEXT("Com_AABB"));

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	pTransform->Set_Scaled({ 1.f, 1.f, 1.f });

	pModel->Repeat_Animation(fTimeDelta);

	pSensei->Set_FormationAry(m_pOwner->Get_StudentInfo().eFormation, true);

	switch (m_pOwner->Get_StudentInfo().eFormation)
	{
	case FORMATION_FIRST:
		m_vPreTranslation = FIRST_POS;
		break;
	case FORMATION_SECOND:
		m_vPreTranslation = SECOND_POS;
		break;
	case FORMATION_THIRD:
		m_vPreTranslation = THIRD_POS;
		break;
	case FORMATION_FOURTH:
		m_vPreTranslation = FOURTH_POS;
		break;
	default:
		m_vPreTranslation = DEFAULT_POS;
		break;
	}

	pTransform->Set_State(CTransform::STATE_TRANSLATION, m_vPreTranslation);


	if (!m_pBtn->Get_Openwindow())
	{
		if (pAABBcom->CollisionRay())
		{
			if (KEY(LBUTTON, TAP))
			{
				pSensei->Set_FormationAry(m_pOwner->Get_StudentInfo().eFormation, false);
				pState = CFormation_Pick::Create(m_pOwner);
				return pState;
			}
		}
	}
	
	for (_uint i = 0; i < pGameInstance->Get_GameObjectSize(LEVEL_FORMATION, TEXT("Layer_Formation_Student")); ++i)
	{
		CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_FORMATION, TEXT("Layer_Formation_Student"), i);
		if (pStudent->Is_Retire())
			continue;

		if (pStudent != m_pOwner)
		{
			CCollider* pAABB = (CCollider*)pStudent->Get_Component(TEXT("Com_AABB"));
			CCollider* pOwnerAABB = (CCollider*)m_pOwner->Get_Component(TEXT("Com_AABB"));

			if (pOwnerAABB->Collision(pAABB))
			{
				pTransform->Set_Scaled(S_SIZE);
				break;
			}

		}
	}

	return pState;
}

void CFormation_Idle::Exit()
{
	Destroy_Instance();
}

void CFormation_Idle::Get_Btn()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_pBtn = (CFormaton_Btn*)pGameInstance->Get_GameObject(LEVEL_FORMATION, TEXT("Layer_Formation_BackGround"), 3);
}

CFormation_Idle * CFormation_Idle::Create(CStudent * pOwner)
{
	return new CFormation_Idle(pOwner);
}