#pragma once
#include "State.h"

BEGIN(Client)

class CRun final : public
	CState
{
private:
	CRun(CStudent* pOwner);
	virtual ~CRun();

public:
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	void Find_Monster();

public:
	static CRun* Create(CStudent* pOwner);
};

END