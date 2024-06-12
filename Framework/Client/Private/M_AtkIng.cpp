#include "stdafx.h"
#include "..\Public\M_AtkIng.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"
#include "M_Run.h"
#include "M_AtkIng.h"
#include "M_AtkEnd.h"
#include "Student.h"
#include "Sensei.h"

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
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CStudent* pStudent = m_pOwner->Get_FoundStudent();

	CSensei* pSensei = GET_SENSEI;

	if (m_bOnce && pStudent)
	{
		if (!pSensei->Get_SenseiInfo().bEx)
			CGameInstance::Get_Instance()->Play_Sound(L"AR_One", 1, 1.f);

		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Monster_Muzzle"), &m_pOwner);
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Monster_Bullet"), &m_pOwner);
		//pStudent->Set_MinusHp(m_pOwner->Get_MonsterAtk());
		m_bOnce = false;
	}

	pModel->Play_Animation(fTimeDelta);

	if (m_pOwner->Get_Fear())
	{
		return CM_Run::Create(m_pOwner);
	}

	if (nullptr != pStudent)
	{
		pTransform->LookAtLerp(pStudent->Get_StudentTranslation(), 7.f, -fTimeDelta); //반대편을 바라봐야 하므로 마이너스
	}

	if (pModel->Get_isFinished())
	{
		if (nullptr == pStudent)
		{
			pState = CM_Run::Create(m_pOwner);
		}
		else
			pState = CM_AtkEnd::Create(m_pOwner);
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