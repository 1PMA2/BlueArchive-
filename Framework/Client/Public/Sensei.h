#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CStudent;

class CSensei final : public CBase
{
	DECLARE_SINGLETON(CSensei)
private:
	CSensei();
	virtual ~CSensei();

public:
	typedef struct tagSenseiDesc
	{
		CStudent* pStudent[10] = { nullptr };

		CAMERA eCamera;
		_float fTimeSpeed;
		_uint iCost;

		_bool bEx = true;

	}SENSEIINFO;

public:
	SENSEIINFO Get_SenseiInfo() { return m_tSensei; }
	_float Set_TimeSpeed(float f);
	void Set_Camera(CAMERA eCamera) { m_tSensei.eCamera = eCamera; }

public:
	SENSEIINFO m_tSensei;

public:
	virtual void Free() override;
};

END



