#pragma once
#include "State.h"

BEGIN(Client)

class CRun_ToKnee final : public
	CState
{
private:
	CRun_ToKnee(CStudent* pOwner);
	virtual ~CRun_ToKnee();

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CRun_ToKnee* Create(CStudent* pOwner);
};

END