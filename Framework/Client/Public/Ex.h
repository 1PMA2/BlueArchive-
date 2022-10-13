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
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	class CMonster* m_pMonster = nullptr;
	_bool	m_bOnce = true;
	_vector m_vTarget = {};

public:
	static CEx* Create(CStudent* pOwner);
};

END