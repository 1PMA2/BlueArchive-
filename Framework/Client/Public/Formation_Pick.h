#pragma once
#include "State.h"

BEGIN(Client)

class CFormation_Pick final : public
	CState
{
private:
	CFormation_Pick(CStudent* pOwner);
	virtual ~CFormation_Pick() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	_float4 m_vOut = {};

private:
	FORMATION m_eCurrentFormation = FORMATION_END;
	FORMATION m_ePreFormation = FORMATION_END;
	_vector m_vPreTranslation = {};
	_vector m_vCurrentTranslation = {};
public:
	static CFormation_Pick* Create(CStudent* pOwner);
};

END