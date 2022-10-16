#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CBoss_Idle final : public
	CMonster_State
{
private:
	CBoss_Idle(CMonster* pOwner);
	virtual ~CBoss_Idle() = default;

public:
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	int i = 0;

public:
	static CBoss_Idle* Create(CMonster* pOwner);
};

END