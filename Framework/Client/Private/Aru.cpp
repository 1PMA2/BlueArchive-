#include "stdafx.h"
#include "..\Public\Aru.h"

#include "GameInstance.h"

CAru::CAru(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CStudent(pDevice, pContext)
{
}

CAru::CAru(const CAru & rhs)
	: CStudent(rhs)
{
}

HRESULT CAru::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAru::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	m_eStudentInfo.strName = TEXT("Aru");

	m_eStudentInfo.fFireSpeed = 0.5f;
	m_eStudentInfo.iAnimNum = 0;
	m_eStudentInfo.iAtk = 10;
	m_eStudentInfo.iDef = 0;
	m_eStudentInfo.iEx = 50;
	m_eStudentInfo.iExCost = 4;
	m_eStudentInfo.iHp = 100;
	m_eStudentInfo.iMagazine = 5;
	m_eStudentInfo.iRange = 10;
	m_eStudentInfo.iShield = 0;
	

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;	

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pModelCom->Set_CurrentAnimation(m_eStudentInfo.iAnimNum);

	return S_OK;
}

void CAru::Tick(_float fTimeDelta)
{
	if (GetKeyState(VK_UP) & 0x8000)
		m_pTransformCom->Go_Straight(fTimeDelta);

	if (GetKeyState(VK_LEFT) & 0x8000)
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * -1.f);

	if (GetKeyState(VK_RIGHT) & 0x8000)
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);

	if (GetKeyState(VK_DOWN) & 0x8000)
		m_pTransformCom->Go_Backward(fTimeDelta);

	__super::Tick(fTimeDelta);
	
}

void CAru::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CAru::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CAru::SetUp_Components()
{
	__super::SetUp_Components();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Aru_Ex"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAru::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}

void CAru::OnDisable()
{
}

void CAru::OnEnable()
{
}

CAru * CAru::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAru*		pInstance = new CAru(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAru");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CAru::Clone(void * pArg)
{
	CAru*		pInstance = new CAru(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAru");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAru::Free()
{
	__super::Free();
}
