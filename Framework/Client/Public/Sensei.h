#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CStudent;

class CSensei final : public CBase
{
	DECLARE_SINGLETON(CSensei)
public:
	typedef struct tagUserDesc
	{
		CStudent* pStudent[10];

		_uint iTimeSpeed;
		_uint iCost;
		
		_bool bEx = true;

	}SENSEIDESC;

private:
	CSensei();
	~CSensei();

public:
	SENSEIDESC Get_SenseiInfo() { return eSensei; }

public:
	SENSEIDESC eSensei;


};

END

