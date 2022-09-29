#include "stdafx.h"
#include "..\Public\Camera_Gacha.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Sensei.h"
#include "Arona_Sack.h"

CCamera_Gacha::CCamera_Gacha(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Gacha::CCamera_Gacha(const CCamera_Gacha & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Gacha::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Gacha::Initialize(void * pArg)
{

	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* 트랜스폼 컴포넌늩를 추가한다. */
	m_WeaponDesc.eTargetLevel = LEVEL_GACHASCENE;
	m_WeaponDesc.pTargetLayerTag = TEXT("Layer_Camera");
	m_WeaponDesc.iTargetIndex = 0;
	m_WeaponDesc.pTargetModelComTag = TEXT("Com_Model");
	m_WeaponDesc.pBoneName = "Camera001";

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CModel*		pTargetModel = (CModel*)pGameInstance->Get_Component(m_WeaponDesc.eTargetLevel, m_WeaponDesc.pTargetLayerTag, m_WeaponDesc.pTargetModelComTag, m_WeaponDesc.iTargetIndex);
	if (nullptr == pTargetModel)
		return E_FAIL;

	m_pBonePtr = pTargetModel->Find_HierarcyNode(m_WeaponDesc.pBoneName);
	m_pTargetBonePtr = pTargetModel->Find_HierarcyNode("Camera001.Target");
	if (nullptr == m_pBonePtr)
		return E_FAIL;

	Safe_AddRef(m_pBonePtr);

	m_pTargetTransform = (CTransform*)pGameInstance->Get_Component(m_WeaponDesc.eTargetLevel, m_WeaponDesc.pTargetLayerTag, TEXT("Com_Transform"));
	if (nullptr == m_pTargetTransform)
		return E_FAIL;

	Safe_AddRef(m_pTargetTransform);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CCamera_Gacha::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	

	_matrix		ParentMatrix = m_pBonePtr->Get_OffsetMatrix() * m_pBonePtr->Get_CombinedMatrix() * m_pBonePtr->Get_TransformMatrix();
	_float fHeigth;
	fHeigth = (XMVectorGetY((ParentMatrix.r[3])));
	if (fHeigth < 2.2f)
		MoveCamera();
	else
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CArona_Sack* pSack = (CArona_Sack*)pGameInstance->Get_GameObject(LEVEL_GACHASCENE, TEXT("Layer_Arona"), 1);
		
		pSack->Set_Enable(true);

		this->Set_Fov(XMConvertToRadians(m_fOpenFov));
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 3.f, 3.f, 1.f));
		m_pTransformCom->LookAt(XMVectorSet(0.f, 5.15f, 0.f, 1.f));

		
		if (pSack->Get_SackOpen())
		{
			m_fOpenFov -= 0.015f;
			this->Set_Fov(XMConvertToRadians(m_fOpenFov));
		}
	}

	if (FAILED(Bind_PipeLine()))
		return;
}

void CCamera_Gacha::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Gacha::Render()
{
	return S_OK;
}

void CCamera_Gacha::OnDisable()
{
}

void CCamera_Gacha::OnEnable()
{

	
}

void CCamera_Gacha::MoveCamera()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pBonePtr)
		return;

	_matrix		ParentMatrix = m_pBonePtr->Get_OffsetMatrix() * m_pBonePtr->Get_CombinedMatrix() * m_pBonePtr->Get_TransformMatrix();
	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);



	_matrix		ParentTargetMatrix = m_pTargetBonePtr->Get_OffsetMatrix() * m_pTargetBonePtr->Get_CombinedMatrix() * m_pTargetBonePtr->Get_TransformMatrix();
	ParentTargetMatrix.r[0] = XMVector3Normalize(ParentTargetMatrix.r[0]);
	ParentTargetMatrix.r[1] = XMVector3Normalize(ParentTargetMatrix.r[1]);
	ParentTargetMatrix.r[2] = XMVector3Normalize(ParentTargetMatrix.r[2]);

	
	_float fHeigth;
	fHeigth = (XMVectorGetY((ParentMatrix.r[3])));

	if(fHeigth > 0.7f && fHeigth < 0.8f)
	{
		m_fFov -= 0.13f;
		this->Set_Fov(XMConvertToRadians(m_fFov));
	}



	XMStoreFloat4x4(&m_WorldMatrix, ParentMatrix * m_pTargetTransform->Get_WorldMatrix());
	XMStoreFloat4x4(&m_TargetWorldMatrix, ParentTargetMatrix * m_pTargetTransform->Get_WorldMatrix());

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4x4(&m_WorldMatrix).r[CTransform::STATE_TRANSLATION]);
	
	m_pTransformCom->LookAt(XMLoadFloat4x4(&m_TargetWorldMatrix).r[CTransform::STATE_TRANSLATION]);

	Safe_Release(pGameInstance);
}

CCamera_Gacha * CCamera_Gacha::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Gacha*		pInstance = new CCamera_Gacha(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Gacha");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Gacha::Clone(void * pArg)
{
	CCamera_Gacha*		pInstance = new CCamera_Gacha(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Gacha");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Gacha::Free()
{
	__super::Free();

	Safe_Release(m_pBonePtr);
	Safe_Release(m_pTargetTransform);
}

