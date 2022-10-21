#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect_Instance;
END

BEGIN(Client)

class CMuzzle final : public CGameObject
{
public:
	CMuzzle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMuzzle(const CMuzzle& rhs);
	virtual ~CMuzzle() = default;

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
	CVIBuffer_Rect_Instance*		m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

public:
	static CMuzzle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END