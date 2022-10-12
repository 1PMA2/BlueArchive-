#include "stdafx.h"
#include "..\Public\Camera_Ex.h"
#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Sensei.h"

CCamera_Ex::CCamera_Ex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Ex::CCamera_Ex(const CCamera_Ex & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Ex::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Ex::Initialize(void * pArg)
{
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(10.f, 5.f, 0.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 5.5f, 1.f);
	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fFovy = XMConvertToRadians(25.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.01f;
	CameraDesc.fFar = 300.f;


	/* ���� ���ϴ� ī�޶��� �ʱ���¸� ����ȭ�ϳ�. */
	if (FAILED(__super::Initialize(&CameraDesc)))
		return E_FAIL;

	/* Ʈ������ �����͈p�� �߰��Ѵ�. */

	return S_OK;
}

void CCamera_Ex::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (FAILED(Bind_PipeLine()))
		return;
}

void CCamera_Ex::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Ex::Render()
{
	return S_OK;
}

void CCamera_Ex::OnDisable()
{
}

void CCamera_Ex::OnEnable()
{
	
}

void CCamera_Ex::Free()
{
	__super::Free();
}
