#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CRenderer;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Student abstract : public CGameObject
{
	public:
		CUI_Student(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		CUI_Student(const CUI_Student& rhs);
		virtual ~CUI_Student() = default;

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
		_float			m_fLocationX, m_fLocationY;

	private:
		HRESULT SetUp_Components();
		HRESULT SetUp_ShaderResource();

	public:
		virtual void OnDisable() override;
		virtual void OnEnable() override;

	public:
		virtual void Free() override;
	};

END