#include "stdafx.h"
#include "..\Public\M_AtkIng.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"
#include "M_Run.h"
#include "M_AtkIng.h"
#include "Student.h"

CM_AtkIng::CM_AtkIng(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(13);
}

void CM_AtkIng::Enter()
{

}

CMonster_State * CM_AtkIng::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CStudent* pStudent = m_pOwner->FoundStudent();

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	pModel->Play_Animation(fTimeDelta);


	if (nullptr != pStudent)
	{
		pTransform->LookAtLerp(pStudent->Get_StudentTranslation(), 5.f, -fTimeDelta); //�ݴ����� �ٶ���� �ϹǷ� ���̳ʽ�

		/*if (m_bOnce)
		{
			m_pOwner->Use_Bullet();
			pMonster->Set_MinusHp(m_pOwner->Get_StudentInfo().iAtk);
			m_bOnce = false;
		}*/

	}

	if (pModel->Get_isFinished())
	{
		if (nullptr == pStudent)
		{
			pState = CM_Run::Create(m_pOwner);
		}
		else
			pState = CM_AtkIng::Create(m_pOwner);
		//else
		//	pState = CHide_ReloadStart::Create(m_pOwner);

		return pState;
	}

	return pState;
}

void CM_AtkIng::Exit()
{
	Destroy_Instance();
}

CM_AtkIng * CM_AtkIng::Create(CMonster * pOwner)
{
	return new CM_AtkIng(pOwner);
}