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

class CAru final : public CStudent
{
public:
	CAru(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAru(const CAru& rhs);
	virtual ~CAru() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();
	_tchar					m_szFPS[MAX_PATH] = TEXT("");

private:
	HRESULT FormationLevel_Collision() override;
	HRESULT GamePlayLevel_Collision() override;
	
public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	_bool	m_bExReady = false;

public:
	static CAru* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END