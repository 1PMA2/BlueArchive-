#include "stdafx.h"
#include "..\Public\M_Run.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"
#include "M_Runend.h"

CM_Run::CM_Run(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(9);
}

void CM_Run::Enter()
{

}

CMonster_State * CM_Run::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Repeat_Animation(fTimeDelta);

	pTransform->Go_Backward(fTimeDelta);

	if (m_pOwner->FoundStudent()) //�л��� ã����
	{
		pState = CM_Runend::Create(m_pOwner);
	}

		


	return pState;
}

void CM_Run::Exit()
{
	Destroy_Instance();
}

CM_Run * CM_Run::Create(CMonster * pOwner)
{
	return new CM_Run(pOwner);
}