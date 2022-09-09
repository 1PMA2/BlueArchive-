#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Client)

class CState;

class CStudent abstract : public CGameObject
{
public:
	typedef struct tagStudentDesc
	{
		wstring	strName;

		_uint	iHp;
		_uint	iShield;
		_uint	iAtk;
		_uint	iDef; //엄폐시 데미지 입을 확률
		_uint	iEx; //스킬공격, 실드, 힐 여러곳에서 사용
		_uint	iExCost;

		_uint	iMagazine;
		_uint	iRange;

		_float	fFireSpeed;

		_uint	iAnimNum;

	}STUDENTINFO;

public:
	CStudent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStudent(const CStudent& rhs);
	virtual ~CStudent() = default;

public:
	

private:


private:



public:
	virtual void Free() override;
};

END