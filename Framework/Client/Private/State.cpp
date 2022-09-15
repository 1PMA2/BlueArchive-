#include "stdafx.h"
#include "..\Public\State.h"

#include "Student.h"
CState::CState(CStudent* pOwner)
	:m_pOwner(pOwner)
{
}


CState::~CState()
{
}

void CState::Set_State(ANIM eState)
{
	 m_pOwner->Set_State(eState);
}
