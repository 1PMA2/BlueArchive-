#include "stdafx.h"
#include "..\Public\M_Landing.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"

#include "M_Run.h"

CM_Landing::CM_Landing(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(3);
}

void CM_Landing::Enter()
{
	
}

CMonster_State * CM_Landing::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);


	if (pModel->Get_isFinished())
	{
		pState = CM_Run::Create(m_pOwner);
	}

	return pState;
}

void CM_Landing::Exit()
{
	Destroy_Instance();
}

CM_Landing * CM_Landing::Create(CMonster * pOwner)
{
	return new CM_Landing(pOwner);
}