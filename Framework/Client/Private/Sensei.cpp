#include "stdafx.h"
#include "..\Public\Sensei.h"
#include "GameInstance.h"
#include "Student.h"

IMPLEMENT_SINGLETON(CSensei)

CSensei::CSensei()
{
	
}


void CSensei::Set_Student(_tchar* pStudentTag, CStudent* pStudent)
{
	m_Student.emplace(pStudentTag, pStudent);
}

void CSensei::Set_FormationStudents(_uint iIndex, _tchar* pName)
{
	pFormationStudents[iIndex] = pName;
}

CStudent * CSensei::Get_Student(_tchar* pStudentTag)
{
	auto	iter = find_if(m_Student.begin(), m_Student.end(), CTag_Finder(pStudentTag));

	if (iter == m_Student.end())
		return nullptr;

	return iter->second;
}

_bool CSensei::Useable_Ex(_float fUseCost) //사용할 코스트
{
	if (fUseCost <= m_tSensei.fCost)
		return true;
	else
		return false;
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
	for (auto& Pair : m_Student)
		Safe_Release(Pair.second);

	m_Student.clear();
}
