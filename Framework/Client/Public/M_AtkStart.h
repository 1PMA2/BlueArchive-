#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CM_AtkStart final : public
	CMonster_State
{
private:
	CM_AtkStart(CMonster* pOwner);
	virtual ~CM_AtkStart() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CM_AtkStart* Create(CMonster* pOwner);
};

END