#include "stdafx.h"
#include "..\Public\Run_ToKnee.h"

#include "GameInstance.h"
#include "Student.h"
#include "ForkLift.h"
#include "Model.h"
#include "Hide_Idle.h"
#include "Run.h"
#include "Knee_ZoomStart.h"
#include "Knee_Reload.h"


CRun_ToKnee::CRun_ToKnee(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_RUNTOKNEE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}


CRun_ToKnee::~CRun_ToKnee()
{
}

void CRun_ToKnee::Enter()
{

}

CState * CRun_ToKnee::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		if (0 >= m_pOwner->Get_StudentInfo().iBullet)
			pState = CKnee_Reload::Create(m_pOwner);
		else
			pState = CKnee_ZoomStart::Create(m_pOwner);
	}


	return pState;
}

void CRun_ToKnee::Exit()
{
	Destroy_Instance();
}

CRun_ToKnee * CRun_ToKnee::Create(CStudent * pOwner)
{
	return new CRun_ToKnee(pOwner);
}