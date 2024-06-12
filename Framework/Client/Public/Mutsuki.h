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
	_bool Get_Card() { return m_bCardSelect; }
	void Set_Card(_bool bCard) { m_bCardSelect = bCard; }

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
	HRESULT FormationLevel_Collision(_float fTimeDelta) override;
	HRESULT GamePlayLevel_Collision(_float fTimeDelta) override;
	_tchar					m_szFPS[MAX_PATH] = TEXT("");
	
public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	_bool	m_bExReady = false;
	_bool	m_bCardSelect = false;

public:
	static CMutsuki* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END