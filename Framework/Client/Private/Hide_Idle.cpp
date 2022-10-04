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

	switch (pOwner->Get_StudentInfo().iIndex)
	{
	case 0:
		m_eAnim = ANIM_HIDEIDLE;
		pOwner->Set_State(m_eAnim);
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case 1:
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
	
	

	if (KEY(Q, TAP))
	{
		pTransform->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 10.f, 1.f));
		pTransform->Set_State(CTransform::STATE_RIGHT, XMVectorSet(1.f, 0.f, 0.f, 0.f));
		pTransform->Set_State(CTransform::STATE_UP, XMVectorSet(0.f, 1.f, 0.f, 0.f));
		pTransform->Set_State(CTransform::STATE_LOOK, XMVectorSet(0.f, 0.f, 1.f, 0.f));
		pState = CRun::Create(m_pOwner);
	}
	else 
	{
		pState = CHide_FireStart::Create(m_pOwner, m_pTarget, m_pCover);
	}
	

	

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
