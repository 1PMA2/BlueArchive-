#include "stdafx.h"
#include "..\Public\Knee_ZoomFire.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Knee_ZoomEnd.h"
#include "Knee_Idle.h"
#include "Ex_Cutin.h"
#include "Sensei.h"
#include "Monster.h"
#include "Run.h"

CKnee_ZoomFire::CKnee_ZoomFire(CStudent* pOwner)
	:CState(pOwner)
{
	
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		m_eAnim = ANIM_KNEEZOOMFIRE;
		pOwner->Set_State(m_eAnim);
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(10);
		break;
	case 2:
		break;

	}
}

void CKnee_ZoomFire::Enter()
{
	m_bOnce = true;
}

CState * CKnee_ZoomFire::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CMonster* pMonster = m_pOwner->FoundMonster();

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	pModel->Play_Animation(fTimeDelta);
	if (nullptr != pMonster)
	{
		pTransform->LookAtLerp(pMonster->Get_MonsterTranslation(), 5.f, fTimeDelta);

		if (m_bOnce)
		{
			m_pOwner->Use_Bullet();
			pMonster->Set_MinusHp(m_pOwner->Get_StudentInfo().iAtk);
			m_bOnce = false;
		}

	}

	if (pModel->Get_isFinished())
	{
		if (nullptr == pMonster)
		{
			pState = CRun::Create(m_pOwner);
		}
		else if (0 < m_pOwner->Get_StudentInfo().iBullet)
		{
			switch (m_pOwner->Get_StudentInfo().eStudent)
			{
			case ARU:
				pState = CKnee_ZoomFire::Create(m_pOwner);
				break;
			case MUTSUKI:
				pState = CKnee_Idle::Create(m_pOwner);
				break;
			case 3:

				break;
			}
			
		}
		else
			pState = CKnee_ZoomEnd::Create(m_pOwner);

		return pState;
	}

	if (Ex())
	{
		pState = CEx_Cutin::Create(m_pOwner);
	}

	return pState;
}

void CKnee_ZoomFire::Exit()
{
	Destroy_Instance();
}

_bool CKnee_ZoomFire::Ex()
{
	CSensei* pSensei = CSensei::Get_Instance();

	if (pSensei->Useable_Ex(m_pOwner->Get_StudentInfo().fExCost))
	{
		if (KEY(SPACE, HOLD))
			return true;
	}
	return false;
}

CKnee_ZoomFire * CKnee_ZoomFire::Create(CStudent * pOwner)
{
	return new CKnee_ZoomFire(pOwner);
}