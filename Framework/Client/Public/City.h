#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
END

BEGIN(Client)

class CCity final : public CGameObject
{
public:
	CCity(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCity(const CCity& rhs);
	virtual ~CCity() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();


public:
	static CCity* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END