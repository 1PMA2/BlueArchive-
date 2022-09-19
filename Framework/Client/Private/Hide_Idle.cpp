#include "stdafx.h"
#include "..\Public\Hide_Idle.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Hide_FireStart.h"

CHide_Idle::CHide_Idle(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_HIDEIDLE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
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

	
	
	{
		pState = CHide_FireStart::Create(m_pOwner);
	}



	

	

	return pState;
}

void CHide_Idle::Exit()
{
	Destroy_Instance();
}

CHide_Idle * CHide_Idle::Create(CStudent * pOwner)
{
	return new CHide_Idle(pOwner);
}
