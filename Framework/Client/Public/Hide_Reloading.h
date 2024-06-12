#pragma once
#include "State.h"

BEGIN(Client)

class CHide_Reloading final : public
	CState
{
private:
	CHide_Reloading(CStudent* pOwner);
	virtual ~CHide_Reloading() = default;

public:
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CHide_Reloading* Create(CStudent* pOwner);
};

END