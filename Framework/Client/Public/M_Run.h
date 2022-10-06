#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CM_Run final : public
	CMonster_State
{
private:
	CM_Run(CMonster* pOwner);
	virtual ~CM_Run() = default;

public:
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	int i = 0;

public:
	static CM_Run* Create(CMonster* pOwner);
};

END