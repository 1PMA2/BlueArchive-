#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Monster.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
class CHierarchyNode;
END

BEGIN(Client)

class CMonster_State;

class CMonster_Boss final : public CMonster
{
public:
	CMonster_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Boss(const CMonster_Boss& rhs);
	virtual ~CMonster_Boss() = default;

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

private:
	class CHierarchyNode* m_pBone = nullptr;

public:
	static CMonster_Boss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END