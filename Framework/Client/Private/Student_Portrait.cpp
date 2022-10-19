#include "stdafx.h"
#include "..\Public\Student_Portrait.h"

#include "GameInstance.h"
#include "Sensei.h"

CStudent_Portrait::CStudent_Portrait(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CStudent_Portrait::CStudent_Portrait(const CStudent_Portrait & rhs)
	: CGameObject(rhs)
{
}

HRESULT CStudent_Portrait::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStudent_Portrait::Initialize(void * pArg)
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

	m_fSizeX = 246.f;
	m_fSizeY = 203.f;
	m_fLocationX = 200.f;
	m_fLocationY = 205.f;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));

	CSensei* pSensei = GET_SENSEI;

	for (_uint i = 0; i < pSensei->Get_StudentNum(); ++i)
	{
		if (pSensei->Get_StudentIndex(i)->Get_StudentInfo().eStudent == m_iImgNum)
		{
			m_iSequence = i;
			break;
		}
	}
	m_fLocationX = ((m_iSequence % 3) + 1.f) * 320;
	m_fLocationY = ((m_iSequence / 3) + 1.f) * 250;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-m_fX + m_fLocationX, m_fY - m_fLocationY, 0.f, 1.f));

	return S_OK;
}

void CStudent_Portrait::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTarget = (CTransform*)pGameInstance->Get_Component(LEVEL_FORMATION, TEXT("Layer_Formation_BackGround"), TEXT("Com_Transform"), 4);

	_float fY = XMVectorGetY(pTarget->Get_State(CTransform::STATE_TRANSLATION)) + 300.f - m_fLocationY;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-m_fX + m_fLocationX, m_fY + fY, 0.f, 1.f));


	ClickPortrait();
	
}

void CStudent_Portrait::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CStudent_Portrait::Render()
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

HRESULT CStudent_Portrait::SetUp_Components()
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

HRESULT CStudent_Portrait::SetUp_ShaderResource()
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

void CStudent_Portrait::ClickPortrait()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_FORMATION, TEXT("Layer_Formation_Student"), m_iSequence);

	CSensei* pSensei = GET_SENSEI;


	for (_uint i = 0; i < 4; ++i)
	{
		if (false == pSensei->Get_FormationAry(i))
		{
			m_iEmptyNum = i; //empty
			break;
		}
	}
	


	if (FORMATION_END != m_pStudent->Get_StudentInfo().eFormation)
	{
		m_bPicked = true;
		m_bOnce = false;
		m_iPickedNum = m_pStudent->Get_StudentInfo().eFormation;
	}

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

	m_pTransformCom->Set_Scaled(_float3(vSize.x, vSize.y, 0.f));

	if (PtInRect(&rcButton, ptMouse))
	{
		if (KEY(LBUTTON, HOLD))
		{
			m_pTransformCom->Set_Scaled(_float3(vSize.x * 1.1f, vSize.y * 1.1f, 0.f));
		}
		else if (KEY(LBUTTON, AWAY))
		{
			m_bPicked = !m_bPicked;
		}
	}

	if (m_bPicked)
	{

		if (m_bOnce)
		{
			m_bOnce = false;
			pSensei->Set_FormationAry(m_iEmptyNum, true);
			m_pStudent->Set_Formation((FORMATION)m_iEmptyNum);
			m_iPickedNum = m_iEmptyNum;
			m_pStudent->Set_Retire(false);
			ENABLE(m_pStudent);
		}
	}
	else if(!m_bOnce)
	{
		m_bOnce = true;
		pSensei->Set_FormationAry(m_iPickedNum, false);
		m_pStudent->Set_Formation(FORMATION_END);
		m_pStudent->Set_Retire(true);
		DISABLE(m_pStudent);
	}

}

void CStudent_Portrait::OnDisable()
{
}

void CStudent_Portrait::OnEnable()
{
}

CStudent_Portrait * CStudent_Portrait::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CStudent_Portrait*		pInstance = new CStudent_Portrait(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStudent_Portrait");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStudent_Portrait::Clone(void * pArg)
{
	CStudent_Portrait*		pInstance = new CStudent_Portrait(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStudent_Portrait");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStudent_Portrait::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}