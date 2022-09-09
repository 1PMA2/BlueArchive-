#include "stdafx.h"
#include "..\Public\Student.h"

#include "GameInstance.h"

CStudent::CStudent(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CStudent::CStudent(const CStudent & rhs)
	: CGameObject(rhs)
{
}

void CStudent::Free()
{
	__super::Free();
}

