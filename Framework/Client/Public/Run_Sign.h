#pragma once
#include "State.h"

BEGIN(Client)

class CRun_Sign final : public
	CState
{
private:
	CRun_Sign(CStudent* pOwner);
	virtual ~CRun_Sign();

public:
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CRun_Sign* Create(CStudent* pOwner);
};

END