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

class CKayoko_Ex final : public CStudent
{
public:
	CKayoko_Ex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKayoko_Ex(const CKayoko_Ex& rhs);
	virtual ~CKayoko_Ex() = default;

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
	CStudent* m_pKayoko = nullptr;

public:
	static CKayoko_Ex* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END