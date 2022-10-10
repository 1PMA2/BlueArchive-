#include "stdafx.h"
#include "..\Public\Formation_Idle.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Formation_Pick.h"
#include "Sensei.h"

CFormation_Idle::CFormation_Idle(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_FORMATIONIDLE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);

	CSensei* pSensei = GET_SENSEI;

	if (0 >= pSensei->Get_FormationInfoSize())
	{
		switch (m_pOwner->Get_StudentInfo().eFormation)
		{
		case FORMATION_FIRST:
			m_vPreTranslation = XMVectorSet(1.5f, 0.f, 0.f, 1.f);
			break;
		case FORMATION_SECOND:
			m_vPreTranslation = XMVectorSet(0.5f, 0.f, 0.f, 1.f);
			break;
		case FORMATION_THIRD:
			m_vPreTranslation = XMVectorSet(-0.5f, 0.f, 0.f, 1.f);
			break;
		case FORMATION_FOURTH:
			m_vPreTranslation = XMVectorSet(-1.5f, 0.f, 0.f, 1.f);
			break;
		case FORMATION_END:
			DISABLE(m_pOwner);
			break;
		}
	}

	else
	{
		for (_uint i = 0; i < pSensei->Get_FormationInfoSize(); ++i)
		{
			if (pSensei->Get_FormationInfo(i).eStudent == m_pOwner->Get_StudentInfo().eStudent)
			{
				m_pOwner->Set_Formation(pSensei->Get_FormationInfo(i).eFormation);

				switch (m_pOwner->Get_StudentInfo().eFormation)
				{
				case FORMATION_FIRST:
					m_vPreTranslation = XMVectorSet(1.5f, 0.f, 0.f, 1.f);
					break;
				case FORMATION_SECOND:
					m_vPreTranslation = XMVectorSet(0.5f, 0.f, 0.f, 1.f);
					break;
				case FORMATION_THIRD:
					m_vPreTranslation = XMVectorSet(-0.5f, 0.f, 0.f, 1.f);
					break;
				case FORMATION_FOURTH:
					m_vPreTranslation = XMVectorSet(-1.5f, 0.f, 0.f, 1.f);
					break;
				}
				
				break;
			}
		}
	}
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

	if (pAABBcom->CollisionRay())
	{
		if (KEY(LBUTTON, TAP))
		{
			pState = CFormation_Pick::Create(m_pOwner);
			return pState;
		}
	}
	
	for (_uint i = 0; i < pGameInstance->Get_GameObjectSize(LEVEL_FORMATION, TEXT("Layer_Formation_Student")); ++i)
	{
		CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_FORMATION, TEXT("Layer_Formation_Student"), i);
		if (pStudent != m_pOwner)
		{
			CCollider* pAABB = (CCollider*)pStudent->Get_Component(TEXT("Com_AABB"));
			CCollider* pOwnerAABB = (CCollider*)m_pOwner->Get_Component(TEXT("Com_AABB"));

			if (pOwnerAABB->Collision(pAABB))
			{
				pTransform->Set_Scaled({ 0.8f, 0.8f, 0.8f });
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

CFormation_Idle * CFormation_Idle::Create(CStudent * pOwner)
{
	return new CFormation_Idle(pOwner);
}