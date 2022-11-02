#include "stdafx.h"
#include "..\Public\Knee_Idle.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Knee_ZoomFire.h"

CKnee_Idle::CKnee_Idle(CStudent* pOwner)
	:CState(pOwner)
{

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));
		m_eAnim = ANIM_KNEEIDLE;
		pOwner->Set_State(m_eAnim);

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		m_fDelay = 1.f;
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(8);
		m_fDelay = 1.f;
		break;
	case HARUKA:
		pModel->Set_CurrentAnimation(3);
		CGameInstance::Get_Instance()->Play_Sound_Player(L"SG_Delay", 0.8f);
		m_fDelay = 1.f;
		break;

	}
}

void CKnee_Idle::Enter()
{
}

CState * CKnee_Idle::Loop(_float fTimeDelta)
{
	CState* pState = __super::Loop(fTimeDelta);

	if (nullptr != pState)
		return pState;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta * m_fDelay);

	if (pModel->Get_isFinished())
	{
		pState = CKnee_ZoomFire::Create(m_pOwner);
	}

	return pState;
}

void CKnee_Idle::Exit()
{
	Destroy_Instance();
}

CKnee_Idle * CKnee_Idle::Create(CStudent * pOwner)
{
	return new CKnee_Idle(pOwner);
}
