#pragma once
#include "State.h"

BEGIN(Client)

class CRun_ToHide final : public
	CState
{
private:
	CRun_ToHide(CStudent* pOwner, CMonster* pTarget, CForkLift* pCover);
	virtual ~CRun_ToHide();

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CRun_ToHide* Create(CStudent* pOwner, CMonster* pTarget, CForkLift* pCover);
};

END