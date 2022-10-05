#include "stdafx.h"
#include "..\Public\Hide_FireStart.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Run.h"
#include "Monster.h"

CHide_FireStart::CHide_FireStart(CStudent* pOwner, CMonster* pTarget, CForkLift* pCover)
	:CState(pOwner, pTarget, pCover)
{
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case 0:
		m_eAnim = ANIM_HIDEFIRESTART;
		pOwner->Set_State(m_eAnim);
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case 1:
		pModel->Set_CurrentAnimation(19);
		break;
	case 2:
		break;

	}
}


void CHide_FireStart::Enter()
{

}

CState * CHide_FireStart::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTTransform;

	if(nullptr != m_pOwner->FoundMonster())
		pTTransform = (CTransform*)m_pOwner->FoundMonster()->Get_Component(TEXT("Com_Transform"));
	else
	{
		pState = CRun::Create(m_pOwner);
		return pState;
	}

	_vector		vTarget = pTTransform->Get_State(CTransform::STATE_TRANSLATION);
	
	pTransform->LookAtLerp(vTarget, 2.f, fTimeDelta);

	if (pModel->Get_isFinished())
	{ 
		pTransform->LookAt(pTTransform->Get_WorldMatrix().r[3]);
		pState = CFire::Create(m_pOwner);
	}

	return pState;
}

void CHide_FireStart::Exit()
{
	Destroy_Instance();
}

CHide_FireStart * CHide_FireStart::Create(CStudent* pOwner, CMonster* pTarget, CForkLift* pCover)
{
	return new CHide_FireStart(pOwner, pTarget, pCover);
}