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

class CBoss_Bullet final : public CGameObject
{
public:
	CBoss_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Bullet(const CBoss_Bullet& rhs);
	virtual ~CBoss_Bullet() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;
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
	static CBoss_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
