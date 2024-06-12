#include "stdafx.h"
#include "..\Public\Boss_Landing.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"

#include "Boss_Move.h"

CBoss_Landing::CBoss_Landing(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Set_CurrentAnimation(0);


}

void CBoss_Landing::Enter()
{

}

CMonster_State * CBoss_Landing::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);


	if (pModel->Get_isFinished())
	{
		pState = CBoss_Move::Create(m_pOwner);
	}

	return pState;
}

void CBoss_Landing::Exit()
{
	Destroy_Instance();
}

CBoss_Landing * CBoss_Landing::Create(CMonster * pOwner)
{
	return new CBoss_Landing(pOwner);
}