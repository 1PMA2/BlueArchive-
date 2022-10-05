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
		pModel->Set_CurrentAnimation(18);
		//m_iAtkFrame = 
		break;
	case 2:
		break;
	}
}


void CFire::Enter()
{
	m_bOnce = true;
}

CState * CFire::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CMonster* pMonster = m_pOwner->FoundMonster();

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	

	pModel->Play_Animation(fTimeDelta);
	if (nullptr != pMonster)
	{
		pTransform->LookAtLerp(pMonster->Get_MonsterTranslation(), 5.f, fTimeDelta);

		if (8 < pModel->Get_CurrentKeyFrame())
		{
			if (m_bOnce)
			{
				m_pOwner->Use_Bullet();
				pMonster->Set_MinusHp(m_pOwner->Get_StudentInfo().iAtk);
				m_bOnce = false;
			}
		}
	}

	if (pModel->Get_isFinished())
	{
		if (nullptr == pMonster)
		{
			m_pOwner->Set_Cover(false);
			pState = CRun::Create(m_pOwner);
		}
		else if(0 < m_pOwner->Get_StudentInfo().iBullet)
			pState = CFire::Create(m_pOwner);
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

void CFire::Exit()
{
	Destroy_Instance();
}

_bool CFire::Ex()
{
	CSensei* pSensei = CSensei::Get_Instance();

	if (pSensei->Useable_Ex(m_pOwner->Get_StudentInfo().fExCost))
	{
		if (KEY(SPACE, HOLD))
			return true;
	}
	return false;
}


CFire * CFire::Create(CStudent * pOwner)
{
	return new CFire(pOwner);
}