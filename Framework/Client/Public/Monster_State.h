#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"

BEGIN(Client)

class CStudent;
class CMonster;
class CForkLift;

class CMonster_State abstract
{
protected:
	CMonster_State(CMonster* pOwner);
	virtual ~CMonster_State();

public:
	virtual	void	Enter() PURE;
	virtual CMonster_State*	Loop(_float fTimeDelta) PURE;
	virtual void	Exit() PURE;

protected:
	CMonster*		m_pOwner;

protected:
	friend class CMonster;
	void	Destroy_Instance() { delete this; }

};

END