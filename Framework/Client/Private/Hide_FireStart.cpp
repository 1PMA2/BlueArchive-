#include "stdafx.h"
#include "..\Public\Hide_FireStart.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"

CHide_FireStart::CHide_FireStart(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_HIDEFIRESTART;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}


void CHide_FireStart::Enter()
{

}

CState * CHide_FireStart::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));



	if (pModel->Get_isFinished())
		pState = CFire::Create(m_pOwner);

	return pState;
}

void CHide_FireStart::Exit()
{
	Destroy_Instance();
}

CHide_FireStart * CHide_FireStart::Create(CStudent * pOwner)
{
	return new CHide_FireStart(pOwner);
}