#pragma once
#include "State.h"

BEGIN(Client)

class CFormation_Idle final : public
	CState
{
private:
	CFormation_Idle(CStudent* pOwner);
	virtual ~CFormation_Idle() = default;

public:
	// CState을(를) 통해 상속됨
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

private:
	class CFormaton_Btn*		m_pBtn = nullptr;
	_vector m_vPreTranslation = {-5, 0, 0, 1};

private:
	void Get_Btn();

public:
	static CFormation_Idle* Create(CStudent* pOwner);
};

END