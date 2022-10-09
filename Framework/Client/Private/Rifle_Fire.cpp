#include "stdafx.h"
#include "..\Public\Rifle_Fire.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Ex_Cutin.h"
#include "Hide_ReloadStart.h"
#include "Sensei.h"
#include "Run.h"
#include "Monster.h"
#include "Rifle_Delay.h"

CRifle_Fire::CRifle_Fire(CStudent* pOwner)
	:CState(pOwner)
{
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));
		m_eAnim = ANIM_FIRE;
		pOwner->Set_State(m_eAnim);

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		m_iAtkFrame = 24;
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(18);
		//m_iAtkFrame = 
		break;
	case 2:
		break;
	}
}


void CRifle_Fire::Enter()
{
	m_bOnce = true;
}

CState * CRifle_Fire::Loop(_float fTimeDelta)
{
	CState* pState = __super::Loop(fTimeDelta);

	if (nullptr != pState)
		return pState;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CMonster* pMonster = m_pOwner->FoundMonster();

	pModel->Play_Animation(fTimeDelta);
	if (nullptr != pMonster)
	{
		pTransform->LookAtLerp(pMonster->Get_MonsterTranslation(), 5.f, fTimeDelta);

			if (m_bOnce)
			{
				m_pOwner->Use_Bullet();
				pMonster->Set_MinusHp(m_pOwner->Get_StudentInfo().iAtk);
				m_bOnce = false;
			}
	}

	if (pModel->Get_isFinished())
	{
		if (nullptr == pMonster)
		{
			pState = CRun::Create(m_pOwner);
		}
		else if (0 < m_pOwner->Get_StudentInfo().iBullet)
			pState = CRifle_Delay::Create(m_pOwner);
		else
			pState = CHide_ReloadStart::Create(m_pOwner);

		return pState;
	}

	if (Ex())
	{
		pState = CEx_Cutin::Create(m_pOwner);
	}

	return pState;
}

void CRifle_Fire::Exit()
{
	Destroy_Instance();
}

_bool CRifle_Fire::Ex()
{
	CSensei* pSensei = CSensei::Get_Instance();

	if (pSensei->Useable_Ex(m_pOwner->Get_StudentInfo().fExCost))
	{
		if (KEY(SPACE, HOLD))
			return true;
	}
	return false;
}


CRifle_Fire * CRifle_Fire::Create(CStudent* pOwner)
{
	return new CRifle_Fire(pOwner);
}