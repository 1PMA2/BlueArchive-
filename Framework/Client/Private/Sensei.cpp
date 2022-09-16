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
		m_tSensei.fTimeSpeed = 1.f;
	}
	if (GetKeyState(VK_F2) & 0x8000)
	{
		m_tSensei.fTimeSpeed = 2.f;
	}
	if (GetKeyState(VK_F3) & 0x8000)
	{
		m_tSensei.fTimeSpeed = 3.f;
	}
	if (GetKeyState(VK_F4) & 0x8000)
	{
		m_tSensei.fTimeSpeed = 0.1f;
	}

}

void CSensei::Tick_Cost(_float fTimeDelta)
{
	if (10.f > m_tSensei.fCost)
		m_tSensei.fCost += (fTimeDelta * 0.5f);
	else
		m_tSensei.fCost = 10.f;
}

void CSensei::Free()
{
}
