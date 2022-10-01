#include "stdafx.h"
#include "..\Public\Sensei.h"
#include "GameInstance.h"
#include "Student.h"

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

void CSensei::Formation_Level(_float fTimeDelta)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CSensei* pSensei = GET_SENSEI;

	_vector vPreTranslation;

	////////////////////////////////////////////////////////////
	pSensei->Set_FormationInfo();

	for (_uint i = 0; i < pSensei->Get_FormationStudentsNum(); ++i)
	{
		CStudent* pStudent = pSensei->Get_FormationStudents(i);

		CTransform* pTransform = (CTransform*)pStudent->Get_Component(TEXT("Com_Transform"));

		if (0 < pSensei->Get_FormationInfoSize())
		{
			for (_uint i = 0; i < pSensei->Get_FormationInfoSize(); ++i)
			{
				if (pSensei->Get_FormationInfo(i).iIndex == pStudent->Get_StudentInfo().iIndex)
				{
					switch (pSensei->Get_FormationInfo(i).eFormation)
					{
					case FORMATION_FIRST:
						vPreTranslation = XMVectorSet(1.5f, 0.f, 0.f, 1.f);
						break;
					case FORMATION_SECOND:
						vPreTranslation = XMVectorSet(0.5f, 0.f, 0.f, 1.f);
						break;
					case FORMATION_THIRD:
						vPreTranslation = XMVectorSet(-0.5f, 0.f, 0.f, 1.f);
						break;
					case FORMATION_FOURTH:
						vPreTranslation = XMVectorSet(-1.5f, 0.f, 0.f, 1.f);
						break;
					}
				}
			}
		}

		if (pStudent->Is_Picked())
		{
			_float4 fOut;
			pGameInstance->Picking((CVIBuffer*)pGameInstance->Get_Component(LEVEL_FORMATION, TEXT("Layer_Formation_BackGround"), TEXT("Com_VIBuffer"), 0),
				(CTransform*)pGameInstance->Get_Component(LEVEL_FORMATION, TEXT("Layer_Formation_BackGround"), TEXT("Com_Transform"), 0), &fOut);

			fOut.y -= 0.5f; // offset
			pTransform->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&fOut));

			if (-0.5f < fOut.y && 1.f > fOut.y)
			{
				if (1.f <= fOut.x)
				{
					pStudent->Set_Formation(FORMATION_FIRST);
					vPreTranslation = XMVectorSet(1.5f, 0.f, 0.f, 1.f);
				}
				else if (1.f > fOut.x && 0.f <= fOut.x)
				{
					pStudent->Set_Formation(FORMATION_SECOND);
					vPreTranslation = XMVectorSet(0.5f, 0.f, 0.f, 1.f);
				}
				else if (0.f > fOut.x && -1.f <= fOut.x)
				{
					pStudent->Set_Formation(FORMATION_THIRD);
					vPreTranslation = XMVectorSet(-0.5f, 0.f, 0.f, 1.f);
				}
				else if (-1.f > fOut.x && -2.f <= fOut.x)
				{
					pStudent->Set_Formation(FORMATION_FOURTH);
					vPreTranslation = XMVectorSet(-1.5f, 0.f, 0.f, 1.f);
				}
			}  //포메이션 위치

		}
		else
			pTransform->Set_State(CTransform::STATE_TRANSLATION, vPreTranslation); //이전위치로

	}

}

void CSensei::Tick_Cost(_float fTimeDelta)
{
	if (10.f > m_tSensei.fCost)
		m_tSensei.fCost += (fTimeDelta * 0.5f);
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

void CSensei::Set_FormationInfo()
{
	m_FormationsInfo.clear();

	auto	iter = m_Formations.begin();

	for (_uint i = 0; i < m_Formations.size(); ++i)
	{
		m_FormationsInfo.push_back((*iter)->Get_StudentInfo());
		++iter;
	}
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

	for (auto& Pair : m_Student)
		Safe_Release(Pair.second);

	m_Student.clear();

}
