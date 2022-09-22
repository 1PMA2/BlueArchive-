#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CStudent;

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
	void Set_TimeSpeed();
	void Set_Camera(CAMERA eCamera) { m_tSensei.eCamera = eCamera; }

	void Set_Student(const _tchar* pStudentTag, CStudent* pStudent);
	void Set_RealStudent(const _tchar* pStudentTag, CStudent* pStudent);
	const _tchar* Get_StudentName(_int iIndex);
	_int Get_StudentNum() { return (_int)m_Student.size(); }
	CStudent* Get_Student(_tchar* pStudentTag);
	CStudent* Get_StudentIndex(_uint iIndex);

	_bool Useable_Ex(_float fUseCost);
	void Use_Ex(_bool bEx, _float fUseCost = 0.f);
	void Tick_Cost(_float fTimeDelta);

public:
	LEVEL m_eCurrentLevel = LEVEL_END;
	SENSEIINFO m_tSensei;

	map<const _tchar*, class CStudent*>					m_Student;
	typedef map<const _tchar*, class CStudent*>			STUDENTS;

	vector<CStudent*>	m_Formations;
	typedef vector<CStudent*> FORMATIONS;

public:
	void Set_FormationStudents(CStudent* pStudents);
	CStudent* Get_FormationStudents(_uint iIndex);



public:
	virtual void Free() override;
};

END



