#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect_Instance;
class CModel;
class CHierarchyNode;
class CCollider;
END

BEGIN(Client)

class CBullet final : public CGameObject
{
public:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CCollider*						m_pSphereCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CVIBuffer_Rect_Instance*		m_pVIBufferCom = nullptr;
private:
	class CStudent*			m_pOwner = nullptr;
	class CHierarchyNode*	m_pBonePtr = nullptr;

private:
	_float4x4				m_WorldMatrix;
	_float					m_fTick = 0.f;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	HRESULT InitLook();

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

public:
	static CBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END