#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CBoss_Landing final : public
	CMonster_State
{
private:
	CBoss_Landing(CMonster* pOwner);
	virtual ~CBoss_Landing() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	int i = 0;

public:
	static CBoss_Landing* Create(CMonster* pOwner);
};

END