#pragma once
#include "State.h"

BEGIN(Client)

class CRifle_Fire final : public
	CState
{
private:
	CRifle_Fire(CStudent* pOwner);
	virtual ~CRifle_Fire() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	_bool Ex();

private:
	_uint m_iAtkFrame = 0;
	_float m_fTimeAcc = 0.f;
	_bool m_bOnce = true;
	_bool m_b0 = true;
	_bool m_b1 = true;
	_bool m_b2 = true;

public:
	static CRifle_Fire* Create(CStudent* pOwner);
};

END
