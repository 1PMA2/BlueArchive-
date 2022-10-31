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

class CHaruka_ExBullet final : public CGameObject
{
public:
	CHaruka_ExBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHaruka_ExBullet(const CHaruka_ExBullet& rhs);
	virtual ~CHaruka_ExBullet() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	class CStudent*			m_pOwner = nullptr;
	_uint						m_iDamage = 0;

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
	static CHaruka_ExBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
