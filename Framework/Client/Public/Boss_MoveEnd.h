#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CBoss_MoveEnd final : public
	CMonster_State
{
private:
	CBoss_MoveEnd(CMonster* pOwner);
	virtual ~CBoss_MoveEnd() = default;

public:
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	int i = 0;

public:
	static CBoss_MoveEnd* Create(CMonster* pOwner);
};

END