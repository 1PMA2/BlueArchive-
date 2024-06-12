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
	_float m_fAtkTime = 0.f;
	_float m_fTimeAcc = 0.f;
	_bool m_bOnce = true;

public:
	static CFire* Create(CStudent* pOwner);
};

END

