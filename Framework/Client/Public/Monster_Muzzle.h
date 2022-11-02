#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_RectX;
class CModel;
class CHierarchyNode;
END

BEGIN(Client)

class CMonster_Muzzle final : public CGameObject
{
public:
	CMonster_Muzzle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Muzzle(const CMonster_Muzzle& rhs);
	virtual ~CMonster_Muzzle() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*						m_pShaderCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CVIBuffer_RectX*		m_pVIBufferCom = nullptr;
private:
	class CMonster*			m_pOwner = nullptr;
	class CHierarchyNode*	m_pBonePtr = nullptr;

private:
	_float4x4				m_WorldMatrix;
	_float	m_fFrame = 0.f;
	_uint	m_iWeapon = 0;
	_uint	m_iMAXFrame = 0;
	_bool   m_bOnce = true;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	HRESULT InitLook();

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

public:
	static CMonster_Muzzle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END