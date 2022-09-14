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
