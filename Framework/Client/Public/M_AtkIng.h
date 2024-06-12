#pragma once
#include "Monster_State.h"

BEGIN(Client)

class CM_AtkIng final : public
	CMonster_State
{
private:
	CM_AtkIng(CMonster* pOwner);
	virtual ~CM_AtkIng() = default;

public:
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CMonster_State * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	_bool m_bOnce = true;

public:
	static CM_AtkIng* Create(CMonster* pOwner);
};

END