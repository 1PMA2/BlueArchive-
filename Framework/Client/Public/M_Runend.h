#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CM_Runend final : public
	CMonster_State
{
private:
	CM_Runend(CMonster* pOwner);
	virtual ~CM_Runend() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	int i = 0;

public:
	static CM_Runend* Create(CMonster* pOwner);
};

END