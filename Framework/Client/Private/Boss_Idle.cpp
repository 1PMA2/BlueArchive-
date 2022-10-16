#include "stdafx.h"
#include "..\Public\Boss_Idle.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"

#include "Boss_IdleToAtk.h"

CBoss_Idle::CBoss_Idle(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Set_CurrentAnimation(3);


}

void CBoss_Idle::Enter()
{

}

CMonster_State * CBoss_Idle::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	pState = CBoss_IdleToAtk::Create(m_pOwner);

	return pState;
}

void CBoss_Idle::Exit()
{
	Destroy_Instance();
}

CBoss_Idle * CBoss_Idle::Create(CMonster * pOwner)
{
	return new CBoss_Idle(pOwner);
}
