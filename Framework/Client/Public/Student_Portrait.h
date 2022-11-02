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

class CStudent_Portrait final : public CGameObject
{
public:
	CStudent_Portrait(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStudent_Portrait(const CStudent_Portrait& rhs);
	virtual ~CStudent_Portrait() = default;

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
	class CStudent*			m_pStudent = nullptr;

private:
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY, m_fTick = 0;
	_float4x4		m_ProjMatrix;

	_bool			m_bPicked = false;
	_int			m_iPickedNum = 0;
	_int			m_iEmptyNum = 0;
	_int			m_iImgNum = 0;
	_int			m_iSequence = 0;
	_float			m_fFade = -1.f;
	_float			m_fLocationX, m_fLocationY;
	_bool			m_bFormationSet = true;


private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	void ClickPortrait();

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

public:
	static CStudent_Portrait* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END