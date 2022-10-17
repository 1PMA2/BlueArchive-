#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CBoss_Idle final : public
	CMonster_State
{
private:
	CBoss_Idle(CMonster* pOwner);
	virtual ~CBoss_Idle() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	_uint m_iRandom = 0;
	CStudent* m_pRandomStudent = nullptr;

public:
	static CBoss_Idle* Create(CMonster* pOwner);
};

END