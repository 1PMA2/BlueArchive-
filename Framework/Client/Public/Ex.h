#pragma once
#include "State.h"

BEGIN(Client)

class CEx final : public
	CState
{
private:
	CEx(CStudent* pOwner);
	virtual ~CEx() = default;

public:
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CEx* Create(CStudent* pOwner);
};

END