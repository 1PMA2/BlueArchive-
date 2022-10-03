#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"

BEGIN(Client)

class CStudent;
class CMonster;

class CState abstract
{
protected:
	CState(CStudent*	pOwner, CMonster* pTarget = nullptr);
	virtual ~CState();
public:
	ANIM	Get_State() { return m_eAnim; }
	void	Set_State(ANIM eState); 
public:
	virtual	void	Enter() PURE;
	virtual CState*	Loop(_float fTimeDelta) PURE;
	virtual void	Exit() PURE;
protected:
	CStudent*		m_pOwner;
	CMonster*		m_pTarget;
	ANIM			m_eAnim = ANIM_RUNSIGN;

protected:
	friend class CStudent;
	void	Destroy_Instance() { delete this; }

};

END