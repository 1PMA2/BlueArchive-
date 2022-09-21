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
	void Set_Student(_tchar* pStudentTag, CStudent* pStudent);
	void Set_FormationStudents(_uint iIndex, _tchar* pName);
	CStudent* Get_Student(_tchar* pStudentTag);
	_bool Useable_Ex(_float fUseCost);
	void Use_Ex(_bool bEx, _float fUseCost = 0.f);
	void Tick_Cost(_float fTimeDelta);

public:
	LEVEL m_eCurrentLevel = LEVEL_END;
	SENSEIINFO m_tSensei;
	_tchar*	pFormationStudents[4];

	map<const _tchar*, class CStudent*>					m_Student;
	typedef map<const _tchar*, class CStudent*>			STUDENTS;




public:
	virtual void Free() override;
};

END



