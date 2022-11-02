#pragma once
#include "Client_Defines.h"
#include "Student.h"
#include "Base.h"

BEGIN(Client)

typedef struct tagSenseiDesc
{	
	CAMERA eCamera = CAMERA_FREE;

	_float fTimeSpeed = 1.f;
	_float fCost = 0.f;

	_bool bEx = false;

}SENSEIINFO;

class CSensei final : public CBase
{
	DECLARE_SINGLETON(CSensei)
private:
	CSensei();
	virtual ~CSensei() = default;


public:
	SENSEIINFO Get_SenseiInfo() { return m_tSensei; }
	_float Get_Cost() { return m_tSensei.fCost; }
	void ReSet_Cost() { m_tSensei.fCost = 0.f; }

	/* Level*/
	void Set_CurrentLevel(LEVEL eLevelID) { m_eCurrentLevel = eLevelID; }
	LEVEL Get_CurrentLevel() { return m_eCurrentLevel; }
	void Set_PreLevel(LEVEL eLevelID) { m_ePreLevel = eLevelID; }
	LEVEL Get_PreLevel() { return m_ePreLevel; }

	/**/
	void Formation_Level(_float fTimeDelta);

	/*event manager*/
	_bool Get_OpenGacha() { return m_bOpenGachaLevel; }
	void Set_OpenGacha(_bool bGachaLevel) { m_bOpenGachaLevel = bGachaLevel; }

	_bool Get_OpenGachaScene() { return m_bOpenGachaScene; }
	void Set_OpenGachaScene(_bool bGachaScene) { m_bOpenGachaScene = bGachaScene; }

	_bool Get_OpenFormationLevel() { return m_bOpenFormationLevel; }
	void Set_OpenFormationLevel(_bool bFormationLevel) { m_bOpenFormationLevel = bFormationLevel; }

	_bool Get_OpenLobbyLevel() { return m_bOpenLobbyLevel; }
	void Set_OpenLobbyLevel(_bool bLobbyLevel) { m_bOpenLobbyLevel = bLobbyLevel; }
	//
	_bool Get_OpenPlayLevel() { return m_bPlayLevel; }
	void Set_OpenPlayLevel(_bool bplayLevel) { m_bPlayLevel = bplayLevel; }

	/*ingame set*/
	void Tick_Cost(_float fTimeDelta);
	void Set_TimeSpeed();
	void Set_Camera(CAMERA eCamera) { m_tSensei.eCamera = eCamera; }
	void Use_Ex(_bool bEx, _float fUseCost = 0.f);
	_bool Useable_Ex(_float fUseCost);
	void Ex_Lockon(CMonster* pTarget);
	CMonster* Get_LockonMonster() const { return m_pTargetMonster; }
	_vector Get_LockonVector();
	void ReSet_Ex();

	/*student*/
	void Set_Student(const _tchar* pStudentTag, CStudent* pStudent);
	void Set_RealStudent(const _tchar* pStudentTag, CStudent* pStudent);
	const _tchar* Get_StudentName(_int iIndex);
	_uint Get_StudentNum() { return (_uint)m_Student.size(); }
	CStudent* Get_Student(_tchar* pStudentTag);
	CStudent* Get_StudentIndex(_uint iIndex); //studentinfo의 index 아님

	_tchar* Get_NewStudent() { return m_pNewStudent; }
	void Set_NewStudent(_tchar* pStudentTag) { m_pNewStudent = pStudentTag; }

public:
	void Set_ExReady(_bool bExReady) { m_bExReady = bExReady; }
	void Set_ExStudent(CStudent* pStudent) { m_pExStudent = pStudent; }
	CStudent* Get_ExStudent() { return m_pExStudent; }
	void Ex_Fire() { m_bExReady = false; }
	_bool Get_ExReady() { return m_bExReady; }
	_bool Get_End() { return m_bEnd; }
	void Set_End(_bool bEnd) { m_bEnd = bEnd; }
	void Set_Sequence(_uint iIdx) { m_iSequence = iIdx; }
	_uint Get_Sequence() { return m_iSequence; }
	void Victory();
	void EndScene(_bool bEndScene) { m_bEndScene = bEndScene; };
	_bool Get_EndScene() { return m_bEndScene; }

	void Retirecount();
	_bool IsFail() { return m_bFail; }
	_bool Get_Retire() { return m_bRetire; }
	void Set_Retire(_bool bRetire) { m_bRetire = bRetire; }

private:
	_bool m_bExReady = false;
	_bool m_bEnd = false;
	_bool m_bEndScene = false;

private:
	CStudent* m_pExStudent = nullptr;
	CMonster* m_pTargetMonster = nullptr;
	_vector m_vExTarget = {};
	_tchar* m_pNewStudent = nullptr;
	LEVEL m_ePreLevel = LEVEL_END;
	LEVEL m_eCurrentLevel = LEVEL_END;
	SENSEIINFO m_tSensei;
	_bool m_bOpenGachaLevel = false;
	_bool m_bOpenGachaScene = false;
	_bool m_bOpenFormationLevel = false;
	_bool m_bOpenLobbyLevel = false;
	_bool m_bFormationOnce = true;
	_uint m_iSequence = 0;
	_uint m_iRetire = 0;
	_bool m_bRetire = false;
	_bool m_bFail = false;
	_bool m_bPlayLevel = false;

	map<const _tchar*, class CStudent*>					m_Student;
	typedef map<const _tchar*, class CStudent*>			STUDENTS;

	vector<CStudent*>	m_Formations; //포메이션 레벨에서만 사용하는 정보
	typedef vector<CStudent*> FORMATIONS;

	vector<CStudent::STUDENTINFO>	m_FormationsInfo; //다른 레벨로 넘겨주는 정보
	typedef vector<CStudent::STUDENTINFO> FORMATIONS_C;

	_bool m_eFormation[4] = { false, false ,false ,false };
public:
	_bool Get_FormationAry(_uint iIdex) { return m_eFormation[iIdex]; }
	void Set_FormationAry(_uint iIdex, _bool eFormation) { m_eFormation[iIdex] = eFormation; }
	void Reset_FormationAry();
public:
	void Set_FormationStudents(CStudent* pStudents);
	CStudent* Get_FormationStudents(_uint iIndex);

	_uint Get_FormationStudentsNum() { 
		if (4 < (_uint)m_Formations.size())
			return 4;
		else
			return (_uint)m_Formations.size();
	} //최대 4

	void Release_FormationStudents() { m_Formations.clear(); }

public:
	void Clear_FormationInfo();
	void Set_FormationInfo(CStudent* pStudent);
	_uint Get_FormationInfoSize() { return (_uint)m_FormationsInfo.size(); }
	CStudent::STUDENTINFO Get_FormationInfo(_uint iIndex);




public:
	virtual void Free() override;
};

END



