#include "stdafx.h"
#include "..\Public\Lobby.h"

CLobby::CLobby(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{

}

CLobby::CLobby(const CLobby & rhs)
	:CGameObject(rhs)
{
}

HRESULT CLobby::Initialize_Prototype()
{

	return E_NOTIMPL;
}

HRESULT CLobby::Initialize(void * pArg)
{
	return E_NOTIMPL;
}

void CLobby::Tick(_float fTimeDelta)
{
}

void CLobby::LateTick(_float fTimeDelta)
{
}

HRESULT CLobby::Render()
{
	return E_NOTIMPL;
}

HRESULT CLobby::SetUp_Components()
{
	return E_NOTIMPL;
}

HRESULT CLobby::SetUp_ShaderResource()
{
	return E_NOTIMPL;
}

CLobby * CLobby::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLobby* pIncetance = new CLobby(pDevice, pContext);

	if (FAILED(pIncetance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLobby");
		Safe_Release(pIncetance);
	}
		return pIncetance;
}

CGameObject * CLobby::Clone(void * pArg)
{
	return nullptr;
}

void CLobby::Free()
{
}
