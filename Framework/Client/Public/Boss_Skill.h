#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CBoss_Skill final : public
	CMonster_State
{
private:
	CBoss_Skill(CMonster* pOwner);
	virtual ~CBoss_Skill() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	int i = 0;

public:
	static CBoss_Skill* Create(CMonster* pOwner);
};

END