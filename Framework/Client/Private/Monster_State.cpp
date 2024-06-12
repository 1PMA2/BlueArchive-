#include "stdafx.h"
#include "..\Public\Monster_State.h"

#include "Student.h"
#include "Monster.h"

CMonster_State::CMonster_State(CMonster* pOwner)
	:m_pOwner(pOwner)
{
}


CMonster_State::~CMonster_State()
{
}
