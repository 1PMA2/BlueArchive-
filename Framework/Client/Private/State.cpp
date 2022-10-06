#include "stdafx.h"
#include "..\Public\State.h"

#include "Student.h"
#include "Monster.h"
#include "ForkLift.h"
#include "Collider.h"

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
	

	return nullptr;
}
