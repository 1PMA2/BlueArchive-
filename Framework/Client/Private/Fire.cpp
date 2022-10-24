#include "stdafx.h"
#include "..\Public\Fire.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Ex_Cutin.h"
#include "Hide_ReloadStart.h"
#include "Sensei.h"
#include "Run.h"
#include "Monster.h"

CFire::CFire(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_FIRE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		m_fAtkTime = 0.8f;
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(18);
		//m_iAtkFrame = 
		break;
	case KAYOKO:
		pModel->Set_CurrentAnimation(10);
		break;
	}
}


void CFire::Enter()
{
	m_bOnce = true;
	m_fTimeAcc = 0.f;
}

CState * CFire::Loop(_float fTimeDelta)
{
	CState* pState = __super::Loop(fTimeDelta);

	if (nullptr != pState)
		return pState;

	m_fTimeAcc += fTimeDelta;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CMonster* pMonster = m_pOwner->FoundMonster();

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	

	pModel->Play_Animation(fTimeDelta);

	if (nullptr != pMonster)
	{
		pTransform->LookAtLerp(pMonster->Get_MonsterTranslation(), 5.f, fTimeDelta);

		if (m_fAtkTime < m_fTimeAcc)
		{
			if (m_bOnce)
			{
				m_pOwner->Use_Bullet();
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Muzzle"), &m_pOwner);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Bullet"), &m_pOwner);

				//pMonster->Set_MinusHp(m_pOwner->Get_StudentInfo().iAtk);
				m_bOnce = false;
			}
		}

	}

	if (pModel->Get_isFinished())
	{
		if (nullptr == pMonster)
		{
			pState = CRun::Create(m_pOwner);
		}
		else if (0 < m_pOwner->Get_StudentInfo().iBullet)
		{
			pState = CFire::Create(m_pOwner);
		}
		else
			pState = CHide_ReloadStart::Create(m_pOwner);

		return pState;
	}

	return pState;
}

void CFire::Exit()
{
	Destroy_Instance();
}




CFire * CFire::Create(CStudent * pOwner)
{
	return new CFire(pOwner);
}