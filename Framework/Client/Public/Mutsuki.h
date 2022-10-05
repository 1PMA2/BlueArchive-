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

class CMutsuki final : public CStudent
{
public:
	CMutsuki(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMutsuki(const CMutsuki& rhs);
	virtual ~CMutsuki() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	HRESULT FormationLevel_Collision() override;
	HRESULT GamePlayLevel_Collision() override;
	_tchar					m_szFPS[MAX_PATH] = TEXT("");
	
public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

public:
	static CMutsuki* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END