#include "stdafx.h"
#include "..\Public\State.h"

#include "Student.h"
#include "Monster.h"
#include "ForkLift.h"
#include "Collider.h"
#include "Sensei.h"
#include "Ex_Cutin.h"
#include "Run.h"

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

	CMonster* pLockonMonster = pSensei->Get_LockonMonster();

	if (nullptr != pLockonMonster)
	{
		pSensei->Ex_Fire(); // ½Ã°£
		if (m_pOwner->Get_ExReady())
		{
			m_pOwner->Set_ExReady(false);
			m_pOwner->Set_Ex(true);
			pState = CEx_Cutin::Create(m_pOwner);
		}
		else
			pState = nullptr;
	}
	

	return pState;
}
