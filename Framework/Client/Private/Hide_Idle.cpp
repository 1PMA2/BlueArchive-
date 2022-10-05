#include "stdafx.h"
#include "..\Public\Hide_Idle.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Hide_FireStart.h"
#include "Run.h"
CHide_Idle::CHide_Idle(CStudent* pOwner, CMonster* pTarget, CForkLift* pCover)
	:CState(pOwner, pTarget, pCover)
{
	

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		m_eAnim = ANIM_HIDEIDLE;
		pOwner->Set_State(m_eAnim);
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(21);
		break;
	case 2:
		break;

	}
}


CHide_Idle::~CHide_Idle()
{
}

void CHide_Idle::Enter()
{

}

CState * CHide_Idle::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;


	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);
	

	pState = CHide_FireStart::Create(m_pOwner, m_pTarget, m_pCover);
	

	

	return pState;
}

void CHide_Idle::Exit()
{
	Destroy_Instance();
}

CHide_Idle * CHide_Idle::Create(CStudent* pOwner, CMonster* pTarget, CForkLift* pCover)
{
	return new CHide_Idle(pOwner, pTarget, pCover);
}
