#pragma once

#include "Client_Defines.h"
#include "GameObject.h"  //10

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CBoss_Dead final : public CGameObject
{

public:
	CBoss_Dead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Dead(const CBoss_Dead& rhs);
	virtual ~CBoss_Dead() = default;


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
	static CBoss_Dead* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END