#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Camera.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CHierarchyNode;
END

BEGIN(Client)

class CCamera_Ex abstract : public CCamera
{
public:
	CCamera_Ex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Ex(const CCamera_Ex& rhs);
	virtual ~CCamera_Ex() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	_float4x4				m_WorldMatrix;
	_float4x4				m_TargetWorldMatrix;

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

protected:
	class CHierarchyNode*	m_pBonePtr = nullptr;
	class CHierarchyNode*	m_pTargetBonePtr = nullptr;
	class CTransform*		m_pTargetTransform = nullptr;

public:
	virtual void Free() override;
};

END