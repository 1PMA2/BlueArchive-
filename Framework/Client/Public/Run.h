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
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	CState* Find_Monster(_float fTimeDelta);

private:
	class CMonster* m_pTargetMonster = nullptr;
	class CForkLift* m_pTargetCover = nullptr;
	_float m_fMin = 9999.f;
	_float m_fCoverMin = 9999.f;

public:
	static CRun* Create(CStudent* pOwner);
};

END