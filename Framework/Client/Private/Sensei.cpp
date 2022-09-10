#include "stdafx.h"
#include "..\Public\Sensei.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CSensei)

CSensei::CSensei()
{
	ZeroMemory(&m_tSensei, sizeof(SENSEIINFO));
}

CSensei::~CSensei()
{
	
}

_float CSensei::Set_TimeSpeed(float f)
{
	m_tSensei.fTimeSpeed = f;

	return m_tSensei.fTimeSpeed;
}

void CSensei::Free()
{
}
