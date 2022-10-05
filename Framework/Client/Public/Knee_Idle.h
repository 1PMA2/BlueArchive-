#pragma once
#include "State.h"

BEGIN(Client)

class CKnee_Idle final : public
	CState
{
private:
	CKnee_Idle(CStudent* pOwner);
	virtual ~CKnee_Idle() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CKnee_Idle* Create(CStudent* pOwner);
};

END
