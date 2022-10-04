#include "stdafx.h"
#include "..\Private\Run_ToHide.h"

#include "GameInstance.h"
#include "Student.h"
#include "ForkLift.h"
#include "Model.h"
#include "Hide_Idle.h"
#include "Run.h"

CRun_ToHide::CRun_ToHide(CStudent* pOwner, CMonster* pTarget, CForkLift* pCover)
	:CState(pOwner, pTarget, pCover)
{
	m_eAnim = ANIM_RUNTOHIDE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}


CRun_ToHide::~CRun_ToHide()
{
}

void CRun_ToHide::Enter()
{
	
}

CState * CRun_ToHide::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CTransform* pCoverT = (CTransform*)m_pCover->Get_Component(TEXT("Com_Transform"));

	pTransform->Set_State(CTransform::STATE_TRANSLATION, pCoverT->Get_State(CTransform::STATE_TRANSLATION));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);
	
	if (pModel->Get_isFinished())
	{
		pState = CHide_Idle::Create(m_pOwner);
	}
	

	return pState;
}

void CRun_ToHide::Exit()
{
	Destroy_Instance();
}

CRun_ToHide * CRun_ToHide::Create(CStudent * pOwner, CMonster* pTarget, CForkLift* pCover)
{
	return new CRun_ToHide(pOwner, pTarget, pCover);
}