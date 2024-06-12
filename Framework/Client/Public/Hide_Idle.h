#pragma once
#include "State.h"

BEGIN(Client)

class CHide_Idle final : public
	CState
{
private:
	CHide_Idle(CStudent* pOwner);
	virtual ~CHide_Idle();

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CHide_Idle* Create(CStudent* pOwner);
};

END