#include "stdafx.h"
#include "..\Public\Boss_AtkToIdle.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"

#include "Boss_Idle.h"

CBoss_AtkToIdle::CBoss_AtkToIdle(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Set_CurrentAnimation(4);

}

void CBoss_AtkToIdle::Enter()
{

}

CMonster_State * CBoss_AtkToIdle::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
		pState = CBoss_Idle::Create(m_pOwner);

	return pState;
}

void CBoss_AtkToIdle::Exit()
{
	Destroy_Instance();
}

CBoss_AtkToIdle * CBoss_AtkToIdle::Create(CMonster * pOwner)
{
	return new CBoss_AtkToIdle(pOwner);
}
