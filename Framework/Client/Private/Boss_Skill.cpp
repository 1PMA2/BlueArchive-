#include "stdafx.h"
#include "..\Public\Boss_Skill.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"

#include "Boss_MoveEnd.h"
#include "Boss_Idle.h"

CBoss_Skill::CBoss_Skill(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Set_CurrentAnimation(9);


}

void CBoss_Skill::Enter()
{

}

CMonster_State * CBoss_Skill::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished()) 
	{
		pState = CBoss_Idle::Create(m_pOwner);
	}

	return pState;
}

void CBoss_Skill::Exit()
{
	Destroy_Instance();
}

CBoss_Skill * CBoss_Skill::Create(CMonster * pOwner)
{
	return new CBoss_Skill(pOwner);
}
