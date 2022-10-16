#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CBoss_Shield final : public
	CMonster_State
{
private:
	CBoss_Shield(CMonster* pOwner);
	virtual ~CBoss_Shield() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	int i = 0;

public:
	static CBoss_Shield* Create(CMonster* pOwner);
};

END