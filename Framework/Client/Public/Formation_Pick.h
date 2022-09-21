#pragma once
#include "State.h"

BEGIN(Client)

class CFormation_Pick final : public
	CState
{
private:
	CFormation_Pick(CStudent* pOwner);
	virtual ~CFormation_Pick() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CFormation_Pick* Create(CStudent* pOwner);
};

END