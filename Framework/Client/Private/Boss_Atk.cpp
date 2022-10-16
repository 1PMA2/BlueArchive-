#include "stdafx.h"
#include "..\Public\Boss_Atk.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"

#include "Boss_AtkToIdle.h"

CBoss_Atk::CBoss_Atk(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Set_CurrentAnimation(5);

}

void CBoss_Atk::Enter()
{

}

CMonster_State * CBoss_Atk::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
		pState = CBoss_AtkToIdle::Create(m_pOwner);

	return pState;
}

void CBoss_Atk::Exit()
{
	Destroy_Instance();
}

CBoss_Atk * CBoss_Atk::Create(CMonster * pOwner)
{
	return new CBoss_Atk(pOwner);
}
