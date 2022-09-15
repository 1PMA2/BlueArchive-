#pragma once
#include "State.h"

BEGIN(Client)

class CEx_Cutin final : public
	CState
{
private:
	CEx_Cutin(CStudent* pOwner);
	virtual ~CEx_Cutin();

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CEx_Cutin* Create(CStudent* pOwner);
};

END
