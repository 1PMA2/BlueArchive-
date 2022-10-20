#include "stdafx.h"
#include "..\Public\Formation_Pick.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Formation_Idle.h"
#include "Sensei.h"

CFormation_Pick::CFormation_Pick(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_FORMATIONPICK;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
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
		pModel->Set_CurrentAnimation(26);
		break;
	case HARUKA:
		pModel->Set_CurrentAnimation(8);
		break;

	}

	switch (m_pOwner->Get_StudentInfo().eFormation)
	{
	case FORMATION_FIRST:
		m_ePreFormation = FORMATION_FIRST;
		m_vPreTranslation = XMVectorSet(1.5f, 0.f, 0.f, 1.f);
		break;
	case FORMATION_SECOND:
		m_ePreFormation = FORMATION_SECOND;
		m_vPreTranslation = XMVectorSet(0.5f, 0.f, 0.f, 1.f);
		break;
	case FORMATION_THIRD:
		m_ePreFormation = FORMATION_THIRD;
		m_vPreTranslation = XMVectorSet(-0.5f, 0.f, 0.f, 1.f);
		break;
	case FORMATION_FOURTH:
		m_ePreFormation = FORMATION_FOURTH;
		m_vPreTranslation = XMVectorSet(-1.5f, 0.f, 0.f, 1.f);
		break;
	}

	pModel->ResetAnimation();
}

void CFormation_Pick::Enter()
{
}

CState * CFormation_Pick::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Repeat_Animation(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	if (KEY(LBUTTON, HOLD))
	{
		pGameInstance->Picking((CVIBuffer*)pGameInstance->Get_Component(LEVEL_FORMATION, TEXT("Layer_Formation_BackGround"), TEXT("Com_VIBuffer"), 1),
			(CTransform*)pGameInstance->Get_Component(LEVEL_FORMATION, TEXT("Layer_Formation_BackGround"), TEXT("Com_Transform"), 1), &m_vOut);

		m_vOut.y -= 0.5f; // offset
		pTransform->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vOut));
	}
	else
	{
		if (-1.0f <= m_vOut.y && 1.f > m_vOut.y)
		{
			if (1.f <= m_vOut.x)
			{
				m_pOwner->Set_Formation(FORMATION_FIRST);
				m_eCurrentFormation = FORMATION_FIRST;
				m_vCurrentTranslation = XMVectorSet(1.5f, 0.f, 0.f, 1.f);

			}
			else if (1.f > m_vOut.x && 0.f <= m_vOut.x)
			{
				m_pOwner->Set_Formation(FORMATION_SECOND);
				m_eCurrentFormation = FORMATION_SECOND;
				m_vCurrentTranslation = XMVectorSet(0.5f, 0.f, 0.f, 1.f);
			}
			else if (0.f > m_vOut.x && -1.f <= m_vOut.x)
			{
				m_pOwner->Set_Formation(FORMATION_THIRD);
				m_eCurrentFormation = FORMATION_THIRD;
				m_vCurrentTranslation = XMVectorSet(-0.5f, 0.f, 0.f, 1.f);
			}
			else if (-1.f > m_vOut.x)
			{
				m_pOwner->Set_Formation(FORMATION_FOURTH);
				m_eCurrentFormation = FORMATION_FOURTH;
				m_vCurrentTranslation = XMVectorSet(-1.5f, 0.f, 0.f, 1.f);
			}


		}  //포메이션 위치
		else
		{
			m_vCurrentTranslation = m_vPreTranslation;
		}
		for (_uint i = 0; i < pGameInstance->Get_GameObjectSize(LEVEL_FORMATION, TEXT("Layer_Formation_Student")); ++i)
		{
			CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_FORMATION, TEXT("Layer_Formation_Student"), i);
			if (pStudent->Is_Retire())
				continue;
			if (pStudent != m_pOwner)
			{
				CCollider* pOwnerAABB = (CCollider*)m_pOwner->Get_Component(TEXT("Com_AABB"));
				CCollider* pAABB = (CCollider*)pStudent->Get_Component(TEXT("Com_AABB"));

				if (pOwnerAABB->Collision(pAABB))
				{
					CTransform* pTargetTransform = (CTransform*)pStudent->Get_Component(TEXT("Com_Transform"));
					pStudent->Set_Formation(m_ePreFormation);
					pTargetTransform->Set_State(CTransform::STATE_TRANSLATION, m_vPreTranslation);
					break;
				}

			}
		}
		CSensei* pSensei = GET_SENSEI;

		pSensei->Clear_FormationInfo();

		pTransform->Set_State(CTransform::STATE_TRANSLATION, m_vCurrentTranslation);

		pState = CFormation_Idle::Create(m_pOwner);
	}

	return pState;

}

void CFormation_Pick::Exit()
{
	Destroy_Instance();
}

CFormation_Pick * CFormation_Pick::Create(CStudent * pOwner)
{
	return new CFormation_Pick(pOwner);
}