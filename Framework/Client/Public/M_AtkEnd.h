#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CM_AtkEnd final : public
	CMonster_State
{
private:
	CM_AtkEnd(CMonster* pOwner);
	virtual ~CM_AtkEnd() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	_bool m_bOnce = true;

public:
	static CM_AtkEnd* Create(CMonster* pOwner);
};

END