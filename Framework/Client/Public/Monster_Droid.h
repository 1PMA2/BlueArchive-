#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Monster.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
END

BEGIN(Client)

class CMonster_State;

class CMonster_Droid final : public CMonster
{
public:
	CMonster_Droid(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Droid(const CMonster_Droid& rhs);
	virtual ~CMonster_Droid() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CMonster_Droid* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END