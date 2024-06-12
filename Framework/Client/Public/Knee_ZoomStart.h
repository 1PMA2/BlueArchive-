#pragma once
#include "State.h"

BEGIN(Client)

class CKnee_ZoomStart final : public
	CState
{
private:
	CKnee_ZoomStart(CStudent* pOwner);
	virtual ~CKnee_ZoomStart() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CKnee_ZoomStart* Create(CStudent* pOwner);
};

END

