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

class CMutsuki_ExBullet final : public CGameObject
{
public:
	CMutsuki_ExBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMutsuki_ExBullet(const CMutsuki_ExBullet& rhs);
	virtual ~CMutsuki_ExBullet() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pSphereACom = nullptr;
	CCollider*				m_pSphereBCom = nullptr;
	CCollider*				m_pSphereCCom = nullptr;
	class CMonster*				m_pTarget = nullptr;
	vector <CMonster*> Monsters;

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	HRESULT Collision_ToMonster();

private:
	HRESULT SetUp_Components();

private:
	void Boom();

private:
	_vector m_vTranslation = {};
	_bool m_bOnce = true;
	_float m_fBoomAcc = 0.f;


public:
	static CMutsuki_ExBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
