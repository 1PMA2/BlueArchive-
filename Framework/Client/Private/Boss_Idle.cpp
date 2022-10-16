#include "stdafx.h"
#include "..\Public\Boss_Idle.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"

#include "Boss_IdleToAtk.h"
#include "Boss_Skill.h"
#include "Boss_Shield.h"
#include "Student.h"

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

	CStudent* pStudent = m_pOwner->Get_FoundStudent();

	if(pStudent)
		pTransform->LookAtLerp(pStudent->Get_StudentTranslation(), 5.f, -fTimeDelta); //반대편을 바라봐야 하므로 마이너스

	pModel->Play_Animation(fTimeDelta * 1.2f);

	if (pModel->Get_isFinished())
	{
		_uint iRandom = random(0, 4);

		switch (iRandom)
		{
		case 0:
			pState = CBoss_Skill::Create(m_pOwner);
			break;
		//case 1:
		//	pState = CBoss_Shield::Create(m_pOwner);
		//	break;
		default:
			pState = CBoss_IdleToAtk::Create(m_pOwner);
			break;
		}
	}

	if (nullptr == pStudent)
		int i = 10;

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
