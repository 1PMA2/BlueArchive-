#pragma once

#include "Student.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
END


BEGIN(Client)

class CStudent;

class CMutsuki_Ex final : public CStudent
{
public:
	CMutsuki_Ex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMutsuki_Ex(const CMutsuki_Ex& rhs);
	virtual ~CMutsuki_Ex() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	CStudent* m_pMutsuki = nullptr;

public:
	static CMutsuki_Ex* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END