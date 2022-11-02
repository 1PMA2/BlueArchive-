#include "stdafx.h"
#include "..\Public\Ex_Portrait.h"

#include "GameInstance.h"
#include "Sensei.h"

Ex_Portrait::Ex_Portrait(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

Ex_Portrait::Ex_Portrait(const Ex_Portrait & rhs)
	: CGameObject(rhs)
{
}

HRESULT Ex_Portrait::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Ex_Portrait::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (nullptr != pArg)
		memcpy(&m_iImgNum, pArg, sizeof(_uint));

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 123.f;
	m_fSizeY = 101.f;
	m_fLocationX = 200.f;
	m_fLocationY = 205.f;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));

	CSensei* pSensei = GET_SENSEI;

	m_iLive = pSensei->Get_FormationInfoSize();

	for (_uint i = 0; i < m_iLive; ++i)
	{
		if (pSensei->Get_FormationInfo(i).eStudent == m_iImgNum)
		{
			m_iSequence = i;

			m_iStudent = i;
			break;
		}
	}
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	
	if (pSensei->Get_FormationInfo(m_iStudent).eFormation == FORMATION_FIRST)
	{
		switch (m_iImgNum)
		{
		case Client::ARU:
			CGameInstance::Get_Instance()->Play_Sound_Rand(L"Aru_Battle_In_", 2, 0.7f);
			break;
		case Client::MUTSUKI:
			CGameInstance::Get_Instance()->Play_Sound_Rand(L"Mutsuki_Battle_In_", 2, 0.7f);
			break;
		case Client::KAYOKO:
			CGameInstance::Get_Instance()->Play_Sound_Rand(L"Kayoko_Battle_In_", 2, 0.7f);
			break;
		case Client::HARUKA:
			CGameInstance::Get_Instance()->Play_Sound_Rand(L"Haruka_Battle_In_", 2, 0.7f);
			break;
		case Client::S_END:
			break;
		default:
			break;
		}
	}

	
	
	m_fLocationY = 630.f;

	return S_OK;
}

void Ex_Portrait::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CSensei* pSensei = GET_SENSEI;


	if (3 != m_iSequence)
		m_fLocationX = m_iSequence * 120;
	else
		m_fLocationX = 600.f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(300.f + m_fLocationX, m_fY - m_fLocationY, 0.f, 1.f));



	ClickPortrait();
}

void Ex_Portrait::LateTick(_float fTimeDelta)
{
	CSensei* pSensei = GET_SENSEI;
	
	if (!pSensei->Get_SenseiInfo().bEx)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}

	if (pSensei->Get_EndScene())
	{
		DELETE(this);
	}
}

HRESULT Ex_Portrait::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT Ex_Portrait::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portrait"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT Ex_Portrait::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
	return E_FAIL;*/
	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Fade", &m_fFade, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iImgNum)))
		return E_FAIL;


	return S_OK;
}

void Ex_Portrait::ClickPortrait()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_iLive = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Icon"));

	m_pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student"), m_iStudent);
	CSensei* pSensei = GET_SENSEI;

	

	_float3 vPos;

	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	vPos.x += (g_iWinCX >> 1);
	vPos.y = -vPos.y + (g_iWinCY >> 1);

	_float3 vSize = _float3(m_fSizeX, m_fSizeY, 0.f);

	RECT rcButton;

	SetRect(&rcButton,
		(_int)(vPos.x - vSize.x * 0.5f),
		(_int)(vPos.y - vSize.y * 0.5f),
		(_int)(vPos.x + vSize.x * 0.5f),
		(_int)(vPos.y + vSize.y * 0.5f));

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rcButton, ptMouse))
	{
		if (KEY(LBUTTON, TAP))
		{
			m_pTransformCom->Set_Scaled(_float3(m_fSizeX * 1.2f, m_fSizeY * 1.2f, 0.f));
			pSensei->Set_Sequence(m_iSequence);
			m_pStudent->Set_Portrait(true);
			
		}
	}

	if (pSensei->Get_SenseiInfo().bEx) // 나머지
	{
		m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
		if (m_pStudent != pSensei->Get_ExStudent())
		{
			if (m_iSequence > pSensei->Get_Sequence())
			{
				if (m_bOnce)
				{
					m_bOnce = false;
					m_iSequence -= 1;
				}
			}
		}
		else
			m_iSequence += (m_iLive - 1) - m_iSequence; //눌린애
	}
	else
	{
		m_bOnce = true;
	}


	if (m_pStudent->Is_Retire())
	{
		pSensei->Set_Sequence(m_iSequence);
		pSensei->Set_Retire(true);
		DELETE(this);
	}
	else
	{
		if (pSensei->Get_Retire())
		{
			if (m_bRetire)
			{
				m_bRetire = false;
				if (m_iSequence > pSensei->Get_Sequence())
				{
					m_iSequence -= 1;
				}
			}
			if (m_iLive - 1 == m_iStudent)
			{
				pSensei->Set_Retire(false);
			}
		}
		else
			m_bRetire = true;
	}
}

void Ex_Portrait::OnDisable()
{
}

void Ex_Portrait::OnEnable()
{
}

Ex_Portrait * Ex_Portrait::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	Ex_Portrait*		pInstance = new Ex_Portrait(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : Ex_Portrait");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * Ex_Portrait::Clone(void * pArg)
{
	Ex_Portrait*		pInstance = new Ex_Portrait(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : Ex_Portrait");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Ex_Portrait::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}