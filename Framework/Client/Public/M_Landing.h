#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CM_Landing final : public
	CMonster_State
{
private:
	CM_Landing(CMonster* pOwner);
	virtual ~CM_Landing() = default;

public:
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	int i = 0;

public:
	static CM_Landing* Create(CMonster* pOwner);
};

END