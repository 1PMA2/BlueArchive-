#include "stdafx.h"
#include "..\Public\Sensei.h"
#include "GameInstance.h"
#include "Student.h"
#include "Monster.h"

IMPLEMENT_SINGLETON(CSensei)

CSensei::CSensei()
{
	
}


void CSensei::Set_Student(const _tchar* pStudentTag, CStudent* pStudent)
{
	m_Student.emplace(pStudentTag, pStudent); //선생이 뽑은 학생
}

void CSensei::Set_RealStudent(const _tchar * pStudentTag, CStudent * pStudent)
{
	auto	iter = find_if(m_Student.begin(), m_Student.end(), CTag_Finder(pStudentTag));
	
	if (iter == m_Student.end())
		return;

	iter->second = pStudent;
}

const _tchar * CSensei::Get_StudentName(_int iIndex)
{
	auto	iter = m_Student.begin();

	for (_int i = 0; i < iIndex; ++i)
		++iter;

	return iter->first;
}

CStudent * CSensei::Get_Student(_tchar* pStudentTag)
{
	auto	iter = find_if(m_Student.begin(), m_Student.end(), CTag_Finder(pStudentTag));

	if (iter == m_Student.end())
		return nullptr;

	return iter->second;
}

CStudent * CSensei::Get_StudentIndex(_uint iIndex) 
{
	auto	iter = m_Student.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return iter->second;
}

_bool CSensei::Useable_Ex(_float fUseCost) //사용할 코스트
{
	if (fUseCost <= m_tSensei.fCost)
		return true;
	else
		return false;
}

_vector CSensei::Get_LockonVector()
{
	vExTarget = m_pTargetMonster->Get_MonsterTranslation();

	m_pTargetMonster = nullptr;

	return vExTarget;
	
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

	if (m_bExReady)
	{
		m_tSensei.fTimeSpeed = 0.1f;
	}
	else
	{
		m_tSensei.fTimeSpeed = 1.f;
	}

}

void CSensei::Formation_Level(_float fTimeDelta)
{

	
}

void CSensei::Tick_Cost(_float fTimeDelta)
{
	if (10.f > m_tSensei.fCost)
		m_tSensei.fCost += (fTimeDelta * 0.2f);
	else
		m_tSensei.fCost = 10.f;
}

void CSensei::Set_FormationStudents(CStudent* pStudents)
{
	m_Formations.push_back(pStudents);
}

CStudent * CSensei::Get_FormationStudents(_uint iIndex)
{
	auto	iter = m_Formations.begin();
	
	for (_uint i = 0; i < iIndex; ++i)
		++iter;
	
	return (*iter);
}

void CSensei::Clear_FormationInfo()
{
	m_FormationsInfo.clear();
}

void CSensei::Set_FormationInfo(CStudent* pStudent)
{
	m_FormationsInfo.push_back(pStudent->Get_StudentInfo());
	/*m_FormationsInfo.clear();

	auto	iter = m_Formations.begin();

	for (_uint i = 0; i < m_Formations.size(); ++i)
	{
		m_FormationsInfo.push_back((*iter)->Get_StudentInfo());
		++iter;
	}*/
}

CStudent::STUDENTINFO CSensei::Get_FormationInfo(_uint iIndex)
{
	auto	iter = m_FormationsInfo.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter);
}

void CSensei::Free()
{
	m_Formations.clear();

	m_FormationsInfo.clear();

	m_Student.clear();

}
