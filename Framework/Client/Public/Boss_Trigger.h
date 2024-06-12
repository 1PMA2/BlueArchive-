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

class CBoss_Trigger final : public CGameObject
{
public:
	CBoss_Trigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Trigger(const CBoss_Trigger& rhs);
	virtual ~CBoss_Trigger() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	HRESULT Collision_ToStudent();

private:
	HRESULT SetUp_Components();

private:
	_vector m_vTranslation = {};


public:
	static CBoss_Trigger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END