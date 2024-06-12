#pragma once
#include "State.h"

BEGIN(Client)

class CKnee_ZoomStart final : public
	CState
{
private:
	CKnee_ZoomStart(CStudent* pOwner);
	virtual ~CKnee_ZoomStart() = default;

public:
	// CState��(��) ���� ��ӵ�
	virtual void Enter() override;
	virtual CState * Loop(_float fTimeDelta) override;
	virtual void Exit() override;

public:
	static CKnee_ZoomStart* Create(CStudent* pOwner);
};

END

