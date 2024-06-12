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

class CLoadingImage final : public CGameObject
{
public:
	CLoadingImage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLoadingImage(const CLoadingImage& rhs);
	virtual ~CLoadingImage() = default;

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
	LEVEL			m_eNextLevel = LEVEL_END;
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY, m_fTick = 0;
	_float4x4		m_ProjMatrix;

	_int			m_iImgNum = 0;
	_int			m_iOne = 1;
	_float			m_fFade = -1.f;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	HRESULT Change_Img();

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

public:
	static CLoadingImage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END