#pragma once
#include "State.h"

BEGIN(Client)

class CFire final : public
	CState
{
private:
	CFire(CStudent* pOwner);
	virtual ~CFire() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	_bool Ex();

public:
	static CFire* Create(CStudent* pOwner);
};

END

