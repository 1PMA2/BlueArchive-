#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking);
public:
	CPicking();
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd, ID3D11DeviceContext* pContext);
	void Compute_RayInWorldSpace();

	_bool Picking(class CCollider* pCollider, class CTransform* pTransform);

private:
	HWND				m_hWnd;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	_vector				m_vRayDir, m_vRayPos;


public:
	virtual void Free() override;
};

END