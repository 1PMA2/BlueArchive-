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

class CAru_ExBullet final : public CGameObject
{
public:
	CAru_ExBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAru_ExBullet(const CAru_ExBullet& rhs);
	virtual ~CAru_ExBullet() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;
	class CMonster*				m_pTarget = nullptr;

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
	static CAru_ExBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
