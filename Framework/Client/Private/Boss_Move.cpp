#include "stdafx.h"
#include "..\Public\Boss_Move.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"

#include "Boss_MoveEnd.h"

CBoss_Move::CBoss_Move(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Set_CurrentAnimation(2);


}

void CBoss_Move::Enter()
{

}

CMonster_State * CBoss_Move::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Repeat_Animation(fTimeDelta);

	pTransform->Go_Backward(fTimeDelta);

	if (m_pOwner->FoundStudent()) //학생을 찾았음
	{
		pState = CBoss_MoveEnd::Create(m_pOwner);
	}

	return pState;
}

void CBoss_Move::Exit()
{
	Destroy_Instance();
}

CBoss_Move * CBoss_Move::Create(CMonster * pOwner)
{
	return new CBoss_Move(pOwner);
}
