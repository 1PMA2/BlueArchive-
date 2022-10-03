#include "stdafx.h"
#include "..\Public\State.h"

#include "Student.h"
#include "Monster.h"

CState::CState(CStudent* pOwner, CMonster* pTarget)
	:m_pOwner(pOwner), m_pTarget(pTarget)
{
}


CState::~CState()
{
}

void CState::Set_State(ANIM eState)
{
	 m_pOwner->Set_State(eState);
}
