#pragma once
#include "State.h"

BEGIN(Client)

class CKnee_Reload final : public
	CState
{
private:
	CKnee_Reload(CStudent* pOwner);
	virtual ~CKnee_Reload() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CKnee_Reload* Create(CStudent* pOwner);
};

END
