#pragma once
#include "State.h"

BEGIN(Client)

class CHide_FireStart final : public
	CState
{
private:
	CHide_FireStart(CStudent* pOwner);
	virtual ~CHide_FireStart() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CHide_FireStart* Create(CStudent* pOwner);
};

END