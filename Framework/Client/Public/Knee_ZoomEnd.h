#pragma once
#include "State.h"

BEGIN(Client)

class CKnee_ZoomEnd final : public
	CState
{
private:
	CKnee_ZoomEnd(CStudent* pOwner);
	virtual ~CKnee_ZoomEnd() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CKnee_ZoomEnd* Create(CStudent* pOwner);
};

END
