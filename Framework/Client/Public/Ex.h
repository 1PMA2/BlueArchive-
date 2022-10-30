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
	_bool	m_bMutsukli = true;
	_bool	m_b0 = true;
	_bool	m_b1 = true;
	_bool	m_b2 = true;
	_bool	m_b3 = true;
	_bool	m_b4 = true;
	_bool	m_b5 = true;
	_bool	m_b6 = true;
	_bool	m_b7 = true;
	_bool	m_b8 = true;

	_vector m_vTarget = {};

public:
	static CEx* Create(CStudent* pOwner);
};

END