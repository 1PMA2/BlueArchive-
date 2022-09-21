#include "..\Public\Picking.h"
#include "Transform.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "VIBuffer.h"

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
	_float4		vProjPos;

	vProjPos.x = ptMouse.x / (Viewport.Width/*Viewport.Width*/ * 0.5f) - 1.f;
	vProjPos.y = ptMouse.y / -(Viewport.Height/*Viewport.Height*/ * 0.5f) + 1.f;
	vProjPos.z = 0.0f;
	vProjPos.w = 1.f;

	if (KEY(X, HOLD))
	{
		int i = 10;
	}
	
	/* 3.뷰스페이스상의 마우스 좌표를 구하자. */
	_vector		vViewPos;

	_matrix    ProjMatrixInv = XMMatrixInverse(nullptr, CPipeLine::Get_Instance()->Get_Transform(CPipeLine::D3DTS_PROJ));


	vViewPos = XMVector3TransformCoord(XMLoadFloat4(&vProjPos), ProjMatrixInv);

	/* 4.마우스레이와 마우스Pos를구하자.  */
	_float4		 vRayDir = { 0.f, 0.f, 0.f ,0.f };
	_float4      vRayPos = { 0.f, 0.f, 0.f ,1.f };
	_float4      vOffSet = { 0.f, 0.f, 0.f ,1.f };

	XMStoreFloat4(&vRayDir, (vViewPos - XMLoadFloat4(&vOffSet)));


	_matrix     ViewMatrixInv = XMMatrixInverse(nullptr, CPipeLine::Get_Instance()->Get_Transform(CPipeLine::D3DTS_VIEW));


	XMStoreFloat4(&m_vRayPos, XMVector3TransformCoord(XMLoadFloat4(&vRayPos), ViewMatrixInv));
	XMStoreFloat4(&m_vRayDir, XMVector3TransformNormal(XMLoadFloat4(&vRayDir), ViewMatrixInv));
}

_bool CPicking::Picking(CVIBuffer * pVIBuffer, CTransform * pTransform, _float4 * pOut)
{
		_matrix         WorldMatrix = XMMatrixInverse(nullptr, pTransform->Get_WorldMatrix());



		_vector		vRayPos = { 0.f,0.f,0.f };
		_vector     vRayDir = { 0.f,0.f,0.f };


		vRayPos = XMVector3TransformCoord(XMLoadFloat4(&m_vRayPos), WorldMatrix);
		vRayDir = XMVector3TransformNormal(XMLoadFloat4(&m_vRayDir), WorldMatrix);

		vRayDir = XMVector3Normalize(vRayDir);


		_uint			iNumFaces = pVIBuffer->Get_NumPrimitive();
		_float4*	    pVerticesPos = pVIBuffer->Get_VerticesPos();

		_float		 fDist;

		for (_uint i = 0; i < iNumFaces; ++i)
		{
			_uint3		iIndices = pVIBuffer->Get_Indices(i);

			_float4		vPickedPos;
			_fvector    vVec1 = XMLoadFloat4(&pVerticesPos[iIndices.ix]);
			GXMVECTOR    vVec2 = XMLoadFloat4(&pVerticesPos[iIndices.iy]);
			HXMVECTOR    vVec3 = XMLoadFloat4(&pVerticesPos[iIndices.iz]);


			if (true == TriangleTests::Intersects(vRayPos, vRayDir, vVec1, vVec2, vVec3, fDist))
			{
				//XMLoadFloat3(&vRayDir) = XMVector3Normalize(XMLoadFloat3(&vRayDir));
				XMStoreFloat4(&vPickedPos, XMVector3Normalize(vRayDir) * fDist + vRayPos);
				XMStoreFloat4(pOut, XMVector3TransformCoord(XMLoadFloat4(&vPickedPos), pTransform->Get_WorldMatrix()));

				return true;
			}
		}

		return false;

}

void CPicking::Free()
{
	Safe_Release(m_pContext);
}