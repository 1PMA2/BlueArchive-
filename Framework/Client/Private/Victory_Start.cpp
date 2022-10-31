#include "stdafx.h"
#include "..\Public\Victory_Start.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Sensei.h"
#include "StateVictory.h"

CVictory_Start::CVictory_Start(CStudent* pOwner)
	:CState(pOwner)
{
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));
	m_eAnim = ANIM_RETIRE;
	pOwner->Set_State(m_eAnim);

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(24);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(24);
		break;
	case KAYOKO:
		pModel->Set_CurrentAnimation(29);
		break;
	case HARUKA:
		pModel->Set_CurrentAnimation(2);
		break;
	}
}

void CVictory_Start::Enter()
{

}

CState * CVictory_Start::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));


	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		pState = CStateVictory::Create(m_pOwner);
	}

	return pState;
}

void CVictory_Start::Exit()
{
	Destroy_Instance();
}

CVictory_Start * CVictory_Start::Create(CStudent * pOwner)
{
	return new CVictory_Start(pOwner);
}