#include "stdafx.h"
#include "..\Public\Boss_Shield.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"

#include "Boss_ShieldToIdle.h"

CBoss_Shield::CBoss_Shield(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Set_CurrentAnimation(13);

}

void CBoss_Shield::Enter()
{

}

CMonster_State * CBoss_Shield::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	

	if (pModel->Get_isFinished())
	{
		//pState = CBoss_ShieldToIdle::Create(m_pOwner);
	}

	return pState;
}

void CBoss_Shield::Exit()
{
	Destroy_Instance();
}

CBoss_Shield * CBoss_Shield::Create(CMonster * pOwner)
{
	return new CBoss_Shield(pOwner);
}
