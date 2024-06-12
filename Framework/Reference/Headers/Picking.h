#pragma once

#include "Base.h"

BEGIN(Engine)
class CVIBuffer;
class CTransform;
class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking);
public:
	CPicking();
	virtual ~CPicking() = default;

public:
	_float4 Get_RayDir() { return m_vRayDir; }
	_float4 Get_RayPos() { return m_vRayPos; }

public:
	HRESULT Initialize(HWND hWnd, ID3D11DeviceContext* pContext);
	void Compute_RayInWorldSpace();


	_bool Picking(CVIBuffer* pVIBuffer, CTransform* pTransform, _float4* pOut);

private:
	HWND				m_hWnd;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	_float4				m_vRayDir, m_vRayPos;


public:
	virtual void Free() override;
};

END