#include "stdafx.h"
#include "..\Public\Camera_Main.h"

#include "GameInstance.h"

CCamera_Main::CCamera_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Main::CCamera_Main(const CCamera_Main & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Main::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Main::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Main::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);

	if (FAILED(Bind_PipeLine()))
		return;
}

void CCamera_Main::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Main::Render()
{
	return S_OK;
}

void CCamera_Main::OnDisable()
{
}

void CCamera_Main::OnEnable()
{
}

CCamera_Main * CCamera_Main::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Main*		pInstance = new CCamera_Main(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Main");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Main::Clone(void * pArg)
{
	CCamera_Main*		pInstance = new CCamera_Main(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Main");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Main::Free()
{
	__super::Free();
}