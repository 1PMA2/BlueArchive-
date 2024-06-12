#pragma once
#include "State.h"

BEGIN(Client)

class CHide_ReloadStart final : public
	CState
{
private:
	CHide_ReloadStart(CStudent* pOwner);
	virtual ~CHide_ReloadStart() = default;

public:
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CHide_ReloadStart* Create(CStudent* pOwner);
};

END