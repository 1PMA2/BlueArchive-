#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CModel;
class CHierarchyNode;
END

BEGIN(Client)

class CSmokeL final : public CGameObject
{
public:
	CSmokeL(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSmokeL(const CSmokeL& rhs);
	virtual ~CSmokeL() = default;

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
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
private:
	_vector			m_vTranslation = {};
	class CHierarchyNode*	m_pBonePtr = nullptr;

private:
	_float4x4				m_ProjMatrix;
	_float	m_fFrame = 1.f;
	_bool	m_bLarge = true;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	HRESULT InitLook();

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

public:
	static CSmokeL* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END