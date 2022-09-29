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

	void Set_CurrentLevel(LEVEL eLevelID) { m_eCurrentLevel = eLevelID; }
	LEVEL Get_CurrentLevel() { return m_eCurrentLevel; }
	void Set_PreLevel(LEVEL eLevelID) { m_ePreLevel = eLevelID; }
	LEVEL Get_PreLevel() { return m_ePreLevel; }

	_bool Get_OpenGacha() { return m_bOpenGachaLevel; }
	void Set_OpenGacha(_bool bGachaLevel) { m_bOpenGachaLevel = bGachaLevel; }

	_bool Get_OpenGachaScene() { return m_bOpenGachaScene; }
	void Set_OpenGachaScene(_bool bGachaScene) { m_bOpenGachaScene = bGachaScene; }

	void Set_TimeSpeed();
	void Set_Camera(CAMERA eCamera) { m_tSensei.eCamera = eCamera; }

	void Set_Student(const _tchar* pStudentTag, CStudent* pStudent);
	void Set_RealStudent(const _tchar* pStudentTag, CStudent* pStudent);
	const _tchar* Get_StudentName(_int iIndex);
	_int Get_StudentNum() { return (_int)m_Student.size(); }
	CStudent* Get_Student(_tchar* pStudentTag);
	CStudent* Get_StudentIndex(_uint iIndex); //studentinfo¿« index æ∆¥‘

	_tchar* Get_NewStudent() { return m_pNewStudent; }
	void Set_NewStudent(_tchar* pStudentTag) { m_pNewStudent = pStudentTag; }

	_bool Useable_Ex(_float fUseCost);
	void Use_Ex(_bool bEx, _float fUseCost = 0.f);
	void Tick_Cost(_float fTimeDelta);

public:
	_tchar* m_pNewStudent = nullptr;
	LEVEL m_ePreLevel = LEVEL_END;
	LEVEL m_eCurrentLevel = LEVEL_END;
	SENSEIINFO m_tSensei;
	CStudent::STUDENTINFO m_tStudentInfo = {};
	_bool m_bOpenGachaLevel = false;
	_bool m_bOpenGachaScene = false;

	map<const _tchar*, class CStudent*>					m_Student;
	typedef map<const _tchar*, class CStudent*>			STUDENTS;

	vector<CStudent*>	m_Formations;
	typedef vector<CStudent*> FORMATIONS;

	vector<CStudent::STUDENTINFO>	m_FormationsInfo;
	typedef vector<CStudent::STUDENTINFO> FORMATIONS_C;

public:
	void Set_FormationStudents(CStudent* pStudents);
	CStudent* Get_FormationStudents(_uint iIndex);
	_uint Get_FormationStudentsNum() { return (_uint)m_Formations.size(); }
	void Release_FormationStudents() { m_Formations.clear(); }

public:
	void Set_FormationInfo();
	_uint Get_FormationInfoSize() { return (_uint)m_FormationsInfo.size(); }
	CStudent::STUDENTINFO Get_FormationInfo(_uint iIndex);



public:
	virtual void Free() override;
};

END



