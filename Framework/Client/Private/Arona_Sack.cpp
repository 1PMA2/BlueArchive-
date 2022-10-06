#include "stdafx.h"
#include "..\Public\Arona_Sack.h"

#include "GameInstance.h"
#include "Fade_Out.h"
#include "BackGround.h"
#include "Sensei.h"
#include "Student_Img.h"
CArona_Sack::CArona_Sack(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CArona_Sack::CArona_Sack(const CArona_Sack & rhs)
	: CGameObject(rhs)
{
}

HRESULT CArona_Sack::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CArona_Sack::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(110.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 5.f, 0.f, 1.f));


	m_pModelCom->Set_CurrentAnimation(1);

	this->Set_Enable(false);


	return S_OK;
}

void CArona_Sack::Tick(_float fTimeDelta)
{
	if(!m_bSackOpen)
	{ 
		if (!m_pModelCom->Get_isFinished())
		{
			m_pModelCom->Play_Animation(fTimeDelta);
		}
		else
		{
			m_pModelCom->Set_CurrentAnimation(1);
		}
	}

	if (KEY(LBUTTON, AWAY))
	{
		m_pModelCom->Set_CurrentAnimation(0);
		m_bSackOpen = true;
	}

	if (m_bSackOpen)
	{
		if (!m_pModelCom->Get_isFinished())
		{
			m_pModelCom->Play_Animation(fTimeDelta);
		}

		m_pTransformCom->Go_Up(fTimeDelta * -0.02f);

		if (m_pTransformCom->TurnFor(XMVectorSet(0.f, -1.f, 0.f, 1.f), fTimeDelta, XMConvertToRadians(180.f)))
		{
			CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			_int iImgNum = 1;

			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHASCENE, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_FadeInOut"), &iImgNum)))
				return;

			Safe_Release(pGameInstance);

			DELETE(this);
		}

		
	}
}

void CArona_Sack::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CArona_Sack::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;




	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		return E_FAIL;*/


		m_pModelCom->Render(i, m_pShaderCom, 0, "g_Bones");
	}

	return S_OK;
}

void CArona_Sack::OnDisable()
{
}

void CArona_Sack::OnEnable()
{
}

HRESULT CArona_Sack::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, -0.7f);

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_AronaSack"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CArona_Sack::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CArona_Sack * CArona_Sack::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CArona_Sack*		pInstance = new CArona_Sack(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArona_CAM");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CArona_Sack::Clone(void * pArg)
{
	CArona_Sack*		pInstance = new CArona_Sack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CArona_Sack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArona_Sack::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);


}