#include "stdafx.h"
#include "..\Public\Camera_Ex.h"
#include "GameInstance.h"
#include "HIerarchyNode.h"

CCamera_Ex::CCamera_Ex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Ex::CCamera_Ex(const CCamera_Ex & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Ex::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Ex::Initialize(void * pArg)
{
	/* 트랜스폼 컴포넌늩를 추가한다. */
	m_WeaponDesc.eTargetLevel = LEVEL_GAMEPLAY;
	m_WeaponDesc.pTargetLayerTag = TEXT("Layer_Student_Ex");
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



	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CCamera_Ex::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pBonePtr)
		return;
	
	_matrix		ParentMatrix = m_pBonePtr->Get_OffsetMatrix() * m_pBonePtr->Get_CombinedMatrix() * m_pBonePtr->Get_TransformMatrix();
	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);
	ParentMatrix.r[3] = XMVector3Normalize(ParentMatrix.r[3]);
	

	_matrix		ParentTargetMatrix = m_pTargetBonePtr->Get_OffsetMatrix() * m_pTargetBonePtr->Get_CombinedMatrix() * m_pTargetBonePtr->Get_TransformMatrix();
	ParentTargetMatrix.r[0] = XMVector3Normalize(ParentTargetMatrix.r[0]);
	ParentTargetMatrix.r[1] = XMVector3Normalize(ParentTargetMatrix.r[1]);
	ParentTargetMatrix.r[2] = XMVector3Normalize(ParentTargetMatrix.r[2]);
	//ParentTargetMatrix.r[3] = XMVector3Normalize(ParentTargetMatrix.r[3]);
	//타겟은 translation 받아오지 말것 fov 십창남


	XMStoreFloat4x4(&m_WorldMatrix, ParentMatrix * m_pTargetTransform->Get_WorldMatrix());
	XMStoreFloat4x4(&m_TargetWorldMatrix, ParentTargetMatrix * m_pTargetTransform->Get_WorldMatrix());

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4x4(&m_WorldMatrix).r[CTransform::STATE_TRANSLATION]);

	m_pTransformCom->LookAt(XMLoadFloat4x4(&m_TargetWorldMatrix).r[CTransform::STATE_TRANSLATION]);

	Safe_Release(pGameInstance);

	if (FAILED(Bind_PipeLine()))
		return;
}

void CCamera_Ex::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Ex::Render()
{
	return S_OK;
}

void CCamera_Ex::OnDisable()
{
}

void CCamera_Ex::OnEnable()
{
}

CCamera_Ex * CCamera_Ex::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Ex*		pInstance = new CCamera_Ex(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Ex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Ex::Clone(void * pArg)
{
	CCamera_Ex*		pInstance = new CCamera_Ex(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Ex");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Ex::Free()
{
	__super::Free();

	Safe_Release(m_pBonePtr);
	Safe_Release(m_pTargetTransform);
}
