#pragma once
#include "State.h"

BEGIN(Client)

class CRifle_Delay final : public
	CState
{
private:
	CRifle_Delay(CStudent* pOwner);
	virtual ~CRifle_Delay() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	_bool Ex();

private:
	_uint m_iAtkFrame = 0;
	_bool m_bOnce = true;

public:
	static CRifle_Delay* Create(CStudent* pOwner);
};

END
