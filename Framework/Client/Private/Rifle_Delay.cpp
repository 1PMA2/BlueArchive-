#include "stdafx.h"
#include "..\Public\Rifle_Delay.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Ex_Cutin.h"
#include "Hide_ReloadStart.h"
#include "Sensei.h"
#include "Run.h"
#include "Monster.h"
#include "Rifle_Fire.h"

CRifle_Delay::CRifle_Delay(CStudent* pOwner)
	:CState(pOwner)
{
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		m_eAnim = ANIM_FIRE;
		pOwner->Set_State(m_eAnim);
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		m_iAtkFrame = 24;
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(16);
		//m_iAtkFrame = 
		break;
	case 2:
		break;
	}
}


void CRifle_Delay::Enter()
{
	m_bOnce = true;
}

CState * CRifle_Delay::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CMonster* pMonster = m_pOwner->FoundMonster();

	pModel->Play_Animation(fTimeDelta);

	if (nullptr != pMonster)
	{
		pTransform->LookAtLerp(pMonster->Get_MonsterTranslation(), 5.f, fTimeDelta);
	}


	if (pModel->Get_isFinished())
	{
		pState = CRifle_Fire::Create(m_pOwner);
	}

	return pState;
}

void CRifle_Delay::Exit()
{
	Destroy_Instance();
}

_bool CRifle_Delay::Ex()
{
	CSensei* pSensei = CSensei::Get_Instance();

	if (pSensei->Useable_Ex(m_pOwner->Get_StudentInfo().fExCost))
	{
		if (KEY(SPACE, HOLD))
			return true;
	}
	return false;
}


CRifle_Delay * CRifle_Delay::Create(CStudent* pOwner)
{
	return new CRifle_Delay(pOwner);
}
