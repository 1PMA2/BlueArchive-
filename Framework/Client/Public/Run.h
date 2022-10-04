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
	void Find_Cover();

private:
	class CMonster* m_pTargetMonster = nullptr;
	class CForkLift* m_pTargetCover[4] = { nullptr };
	_bool m_bOnce = true;
	_uint m_iIndex = 0;
	_float m_fMin = 9999.f;
	_float m_fCoverMin = 9999.f;

public:
	static CRun* Create(CStudent* pOwner);
};

END