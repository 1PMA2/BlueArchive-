#pragma once
#include "State.h"

BEGIN(Client)

class CStateVictory final : public
	CState
{
private:
	CStateVictory(CStudent* pOwner);
	virtual ~CStateVictory() = default;

public:
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	_bool	m_bOnce = true;

public:
	static CStateVictory* Create(CStudent* pOwner);
};

END