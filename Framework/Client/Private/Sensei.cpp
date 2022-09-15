#include "stdafx.h"
#include "..\Public\Sensei.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CSensei)

CSensei::CSensei()
{
}

void CSensei::Use_Ex(_bool bEx, _float fCost)
{
	if (fCost <= m_tSensei.fCost)
	{
		m_tSensei.bEx = bEx;
		m_tSensei.fCost -= fCost;
	}
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

void CSensei::Tick_Cost(_float fTimeDelta)
{
	if (10.f > m_tSensei.fCost)
		m_tSensei.fCost += fTimeDelta;
	else
		m_tSensei.fCost = 10.f;
}

void CSensei::Free()
{
}
