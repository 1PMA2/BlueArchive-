#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CBoss_IdleToAtk final : public
	CMonster_State
{
private:
	CBoss_IdleToAtk(CMonster* pOwner);
	virtual ~CBoss_IdleToAtk() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	int i = 0;

public:
	static CBoss_IdleToAtk* Create(CMonster* pOwner);
};

END