#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CModel;
class CHierarchyNode;
class CCollider;
END

BEGIN(Client)

class CCylinder final : public CGameObject
{
public:
	CCylinder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCylinder(const CCylinder& rhs);
	virtual ~CCylinder() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*						m_pShaderCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CModel*							m_pModelCom = nullptr;

private:
	class CStudent*			m_pOwner = nullptr;
	class CHierarchyNode*	m_pBonePtr = nullptr;

private:
	_float4x4				m_WorldMatrix;
	_vector					m_vT;
	_float					m_fTick = 0.f;
	_float					m_fFade = -1.f;
	_float					m_fSize = 0.f;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	HRESULT InitLook();
	_float Lerp(_float fStart, _float fEnd, _float fTime);

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

public:
	static CCylinder* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END