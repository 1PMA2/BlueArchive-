#include "stdafx.h"
#include "..\Public\State.h"

#include "Student.h"
#include "Monster.h"
#include "ForkLift.h"
#include "Collider.h"
#include "Sensei.h"
#include "Ex_Cutin.h"
#include "Run.h"
#include "Hide_Idle.h"
#include "Retire.h"
CState::CState(CStudent* pOwner, CMonster* pTarget, CForkLift* pCover)
	:m_pOwner(pOwner), m_pTarget(pTarget), m_pCover(pCover)
{
}


CState::~CState()
{
}

void CState::Set_State(ANIM eState)
{
	 m_pOwner->Set_State(eState);
}

CState * CState::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CSensei* pSensei = GET_SENSEI;
	if (m_pOwner->Get_ExReady())
	{
		CMonster* pLockonMonster = pSensei->Get_LockonMonster();

	if (nullptr != pLockonMonster)
	{
		pSensei->Ex_Fire(); // ½Ã°£
		
			m_pOwner->Set_ExReady(false);
			m_pOwner->Set_Ex(true);
			pSensei->Use_Ex(true, m_pOwner->Get_StudentInfo().fExCost);
			pState = CEx_Cutin::Create(m_pOwner);
		}
		else
			pState = nullptr;
	}
	
	if (pSensei->Get_End())
		pState = CHide_Idle::Create(m_pOwner);

	if (0 > m_pOwner->Get_StudentInfo().iHp)
		pState = CRetire::Create(m_pOwner);

	return pState;
}
