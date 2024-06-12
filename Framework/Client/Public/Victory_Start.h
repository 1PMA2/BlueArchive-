#pragma once
#include "State.h"

BEGIN(Client)

class CVictory_Start final : public
	CState
{
private:
	CVictory_Start(CStudent* pOwner);
	virtual ~CVictory_Start() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	_bool	m_bOnce = true;

public:
	static CVictory_Start* Create(CStudent* pOwner);
};

END