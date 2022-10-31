#include "stdafx.h"
#include "..\Public\StateVictory.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Sensei.h"

CStateVictory::CStateVictory(CStudent* pOwner)
	:CState(pOwner)
{
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));
	m_eAnim = ANIM_RETIRE;
	pOwner->Set_State(m_eAnim);

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(23);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(23);
		break;
	case KAYOKO:
		pModel->Set_CurrentAnimation(22);
		break;
	case HARUKA:
		pModel->Set_CurrentAnimation(31);
		break;
	}
}

void CStateVictory::Enter()
{

}

CState * CStateVictory::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Repeat_Animation(fTimeDelta);

	return pState;
}

void CStateVictory::Exit()
{
	Destroy_Instance();
}

CStateVictory * CStateVictory::Create(CStudent * pOwner)
{
	return new CStateVictory(pOwner);
}