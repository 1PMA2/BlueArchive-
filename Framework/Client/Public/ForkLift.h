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

class CForkLift final : public CGameObject
{
public:
	CForkLift(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CForkLift(const CForkLift& rhs);
	virtual ~CForkLift() = default;

public:
	void Set_Use() { m_bUsed = true; }
	_bool Get_Use() { return m_bUsed; }

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
	CCollider*				m_pSphereCom = nullptr;
	CCollider*				m_pMSphereCom = nullptr;
	_bool					m_bUsed = false;
	_bool					m_bExit = false;
	_vector					m_vTranslation = {};

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();
	void Collision_ToPlayer();
	void Collision_ToMonster();
	

public:
	static CForkLift* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END