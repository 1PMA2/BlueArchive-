#include "stdafx.h"
#include "..\Public\Sensei.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CSensei)

CSensei::CSensei()
{
}

CSensei::~CSensei()
{
	
}

void CSensei::Set_TimeSpeed()
{

	

	if (GetKeyState(VK_F1) & 0x8000)
	{
		m_tSensei.fTimeSpeed += 1.f;
	}

	if (3 < m_tSensei.fTimeSpeed)
	{
		m_tSensei.fTimeSpeed = 1.f;
	}
}
