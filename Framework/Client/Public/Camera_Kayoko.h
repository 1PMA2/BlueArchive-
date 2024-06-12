#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Camera_Ex.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CHierarchyNode;

END

BEGIN(Client)

class CCamera_Kayoko final : public CCamera_Ex
{
public:
	CCamera_Kayoko(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Kayoko(const CCamera_Kayoko& rhs);
	virtual ~CCamera_Kayoko() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	void MoveCamera(_float fTimeDelta);

private:
	class CStudent* m_pKayokoEx = nullptr;

public:
	static CCamera_Kayoko* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END