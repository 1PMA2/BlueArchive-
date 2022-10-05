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
	_bool Ex();
	_bool m_bOnce = true;

public:
	static CKnee_ZoomFire* Create(CStudent* pOwner);
};

END