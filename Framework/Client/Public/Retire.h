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
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CRetire* Create(CStudent* pOwner);
};

END