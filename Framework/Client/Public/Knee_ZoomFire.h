#pragma once
#include "State.h"

BEGIN(Client)

class CKnee_ZoomFire final : public
	CState
{
private:
	CKnee_ZoomFire(CStudent* pOwner);
	virtual ~CKnee_ZoomFire() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	_float m_fAtkTime = 0.f;
	_float m_fTimeAcc = 0.f;
	_bool m_bOnce = true;
	_bool m_b0 = true;
	_bool m_b1 = true;
	_bool m_b2 = true;

public:
	static CKnee_ZoomFire* Create(CStudent* pOwner);
};

END