#include "..\Public\Picking.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "GameInstance.h"
#include "PipeLine.h"
IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{
}

HRESULT CPicking::Initialize(HWND hWnd, ID3D11DeviceContext * pContext)
{
	m_hWnd = hWnd;

	m_pContext = pContext;

	Safe_AddRef(m_pContext);

	return S_OK;
}

void CPicking::Compute_RayInWorldSpace()
{
	if (nullptr == m_pContext)
		return;

	/*1. 뷰포트 상의 마우스 좌표를 구하자.  */
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	UINT ViewportCount = 1;
	D3D11_VIEWPORT		Viewport;
	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));
	m_pContext->RSGetViewports(&ViewportCount, &Viewport);
	

	/* 2. 투영 스페이스 상의 마우스 좌표를 구하자. */
	_vector		vProjPos;

	_float x, y, z;

	x = ptMouse.x / (Viewport.Width * 0.5f) - 1.f;
	y = ptMouse.y / -(Viewport.Height * 0.5f) + 1.f;
	z = 0.f;

	vProjPos = XMVectorSet(x, y, z, 1.f);

	/* 3.뷰스페이스상의 마우스 좌표를 구하자. */
	_vector		vViewPos;

	_matrix		ProjMatrixInv;

	CPipeLine*			pPipeLine = GET_INSTANCE(CPipeLine);

	ProjMatrixInv = pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ);
	ProjMatrixInv = XMMatrixInverse(nullptr, ProjMatrixInv);
	vViewPos = XMVector3TransformCoord(vProjPos, ProjMatrixInv);

	/* 4.마우스레이와 마우스Pos를구하자.  */
	_vector		vRayDir, vRayPos;
	vRayDir = vViewPos - XMVectorSet(0, 0, 0, 1);
	vRayPos = XMVectorSet(0, 0, 0, 1);

	/* 5.월드로가자. */
	_matrix		ViewMatrixInv;
	ViewMatrixInv = pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW);
	ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrixInv);

	m_vRayDir = XMVector3TransformNormal(vRayDir, ViewMatrixInv);
	m_vRayPos = XMVector3TransformCoord(vRayPos, ViewMatrixInv);



	RELEASE_INSTANCE(CPipeLine);
}

_bool CPicking::Picking(CCollider* pCollider, CTransform * pTransform)
{
	_matrix		WorldMatrix = pTransform->Get_WorldMatrix();
	WorldMatrix = XMMatrixInverse(nullptr, WorldMatrix);

	_vector			vRayPos, vRayDir;

	vRayPos = XMVector3TransformCoord(m_vRayPos, WorldMatrix);
	vRayDir = XMVector3TransformNormal(m_vRayDir, WorldMatrix);
	vRayDir = XMVector3Normalize(vRayDir);
	

	return pCollider->CollisionRay(vRayPos, vRayDir, 0.f);

}


void CPicking::Free()
{
	Safe_Release(m_pContext);
}