#include "stdafx.h"
#include "..\Public\Boss_MoveEnd.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"

#include "Boss_Idle.h"

CBoss_MoveEnd::CBoss_MoveEnd(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Set_CurrentAnimation(1);


}

void CBoss_MoveEnd::Enter()
{

}

CMonster_State * CBoss_MoveEnd::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	/*if (pModel->Get_isFinished())
	{
		pState = CBoss_Idle::Create(m_pOwner);
	}*/

	return pState;
}

void CBoss_MoveEnd::Exit()
{
	Destroy_Instance();
}

CBoss_MoveEnd * CBoss_MoveEnd::Create(CMonster * pOwner)
{
	return new CBoss_MoveEnd(pOwner);
}
