#pragma once
#include "State.h"

BEGIN(Client)

class CRetire final : public
	CState
{
private:
	CRetire(CStudent* pOwner);
	virtual ~CRetire() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	_bool	m_bOnce = true;

public:
	static CRetire* Create(CStudent* pOwner);
};

END