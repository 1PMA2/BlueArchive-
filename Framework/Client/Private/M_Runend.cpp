#include "stdafx.h"
#include "..\Public\M_Runend.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"
#include "M_AtkStart.h"

CM_Runend::CM_Runend(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(12);
}

void CM_Runend::Enter()
{

}

CMonster_State * CM_Runend::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);


	if (pModel->Get_isFinished())
	{
		pState = CM_AtkStart::Create(m_pOwner);
	}

	return pState;
}

void CM_Runend::Exit()
{
	Destroy_Instance();
}

CM_Runend * CM_Runend::Create(CMonster * pOwner)
{
	return new CM_Runend(pOwner);
}