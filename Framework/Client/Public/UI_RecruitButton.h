#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_RecruitButton final : public CGameObject
{
public:
	CUI_RecruitButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_RecruitButton(const CUI_RecruitButton& rhs);
	virtual ~CUI_RecruitButton() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY, m_fTick = 0;
	_float4x4		m_ProjMatrix;

	_int			m_iImgNum = 0;
	_float			m_fFade = -1.f;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	_bool Random(_float fPercent);

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

public:
	static CUI_RecruitButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

