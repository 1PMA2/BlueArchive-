#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Main final : public CCamera
{
public:
	CCamera_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Main(const CCamera_Main& rhs);
	virtual ~CCamera_Main() = default;

public:
	void Shaking_Camera(_bool bShake) { m_bShake = bShake; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	class CStudent* m_pStudent = nullptr;
	_vector m_vMaxFov = {};
	_vector m_vCurrentFov = {};
	_float m_fFovRatio = 0.f;
	_float m_fMin = 999.f;
	_float m_fMax = 0.f;
	_vector m_vMainCamera = {};
	_float m_fShakeTime = 0.f;
	_bool m_bShake = false;

private:
	void Shake_Camera(_float fTimeDelta);
	void Move_Camera(_float fTimeDelta);
	void Set_FovZ(_float fTimeDelta);

public:
	static CCamera_Main* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END