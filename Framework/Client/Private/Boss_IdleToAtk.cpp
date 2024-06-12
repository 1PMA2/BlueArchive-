#include "stdafx.h"
#include "..\Public\Boss_IdleToAtk.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"

#include "Boss_Atk.h"

CBoss_IdleToAtk::CBoss_IdleToAtk(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Set_CurrentAnimation(6);


}

void CBoss_IdleToAtk::Enter()
{

}

CMonster_State * CBoss_IdleToAtk::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if(pModel->Get_isFinished())
		pState = CBoss_Atk::Create(m_pOwner);

	return pState;
}

void CBoss_IdleToAtk::Exit()
{
	Destroy_Instance();
}

CBoss_IdleToAtk * CBoss_IdleToAtk::Create(CMonster * pOwner)
{
	return new CBoss_IdleToAtk(pOwner);
}
