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

class CBoss_SBullet final : public CGameObject
{
public:
	CBoss_SBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_SBullet(const CBoss_SBullet& rhs);
	virtual ~CBoss_SBullet() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	class CMonster*				m_pOwner = nullptr;

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	void Boom();
	HRESULT Collision_ToMonster();

private:
	HRESULT SetUp_Components();

private:
	_vector m_vTranslation = {};
	_bool m_bOnce = true;
	_float m_fBoomAcc = 0.f;


public:
	static CBoss_SBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
