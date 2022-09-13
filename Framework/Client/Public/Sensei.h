#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CStudent;

	typedef struct tagSenseiDesc
	{
		CStudent* pStudent[10] = {nullptr};

		CAMERA eCamera = CAMERA_FREE;
		_float fTimeSpeed = 1.f;
		_float fCost = 0.f;

		_bool bEx = true;

	}SENSEIINFO;

class CSensei final : public CBase
{
	DECLARE_SINGLETON(CSensei)
private:
	CSensei();
	virtual ~CSensei() = default;

public:
	SENSEIINFO Get_SenseiInfo() { return m_tSensei; }
	void Set_TimeSpeed();
	void Set_Camera(CAMERA eCamera) { m_tSensei.eCamera = eCamera; }
	void Tick_Cost(_float fTimeDelta);

public:
	SENSEIINFO m_tSensei;

public:
	virtual void Free() override;
};

END



