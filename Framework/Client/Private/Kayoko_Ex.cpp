#include "stdafx.h"
#include "..\Public\Kayoko_Ex.h"

#include "GameInstance.h"
#include "Ex_Cutin.h"
#include "Camera.h"
#include "Sensei.h"

CKayoko_Ex::CKayoko_Ex(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CStudent(pDevice, pContext)
{
}

CKayoko_Ex::CKayoko_Ex(const CKayoko_Ex & rhs)
	: CStudent(rhs)
{
}

HRESULT CKayoko_Ex::Initialize_Prototype()
{
	m_tStudentInfo.pName = TEXT("Kayoko_Ex");
	m_tStudentInfo.bExModel = true;
	m_tStudentInfo.eStudent = KAYOKO;

	return S_OK;
}

HRESULT CKayoko_Ex::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	m_tStudentInfo.eAnim = ANIM_EX;
	m_tStudentInfo.eFormation = FORMATION_THIRD;


	if (FAILED(SetUp_Components()))
		return E_FAIL;




	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (nullptr != pArg)
		m_pKayoko = (CStudent*)pArg;

	m_pModelCom->Set_CurrentAnimation(0);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Kayoko"), TEXT("Prototype_GameObject_Camera_Kayoko"), this)))
		return E_FAIL;

	return S_OK;
}

void CKayoko_Ex::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CSensei* pSensei = GET_SENSEI;

	CCamera* pCameraEx = (CCamera*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Kayoko"));

	if (m_pKayoko->Get_Ex())
	{
		ENABLE(pCameraEx);
		m_pModelCom->Play_Animation(fTimeDelta);
	}
	else
	{
		DISABLE(pCameraEx);
		m_pModelCom->ResetAnimation();
	}

	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CKayoko_Ex::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CKayoko_Ex::Render()
{
	CSensei* pSensei = GET_SENSEI;

	if (nullptr != pSensei->Get_ExStudent())
	{
		if (pSensei->Get_ExStudent()->Get_StudentInfo().eStudent == m_tStudentInfo.eStudent)
		{
			__super::Render();
		}
	}

	return S_OK;
}

HRESULT CKayoko_Ex::SetUp_Components()
{
	__super::SetUp_Components();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Kayoko_Ex"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CKayoko_Ex::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}

void CKayoko_Ex::OnDisable()
{
}

void CKayoko_Ex::OnEnable()
{
}

CKayoko_Ex * CKayoko_Ex::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CKayoko_Ex*		pInstance = new CKayoko_Ex(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CKayoko_Ex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CKayoko_Ex::Clone(void * pArg)
{
	CKayoko_Ex*		pInstance = new CKayoko_Ex(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CKayoko_Ex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKayoko_Ex::Free()
{
	__super::Free();
}