#include "stdafx.h"
#include "..\Public\Camera_Kayoko.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Sensei.h"
#include "Student.h"

CCamera_Kayoko::CCamera_Kayoko(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera_Ex(pDevice, pContext)
{
}

CCamera_Kayoko::CCamera_Kayoko(const CCamera_Kayoko & rhs)
	: CCamera_Ex(rhs)
{
}

HRESULT CCamera_Kayoko::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Kayoko::Initialize(void * pArg)
{

	if (nullptr != pArg)
		m_pKayokoEx = (CStudent*)pArg;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CModel*		pTargetModel = (CModel*)m_pKayokoEx->Get_Component(TEXT("Com_Model"));

	m_pBonePtr = pTargetModel->Find_HierarcyNode("Camera001");
	m_pTargetBonePtr = pTargetModel->Find_HierarcyNode("Camera001.Target");

	m_pTargetTransform = (CTransform*)m_pKayokoEx->Get_Component(TEXT("Com_Transform"));


	RELEASE_INSTANCE(CGameInstance);

	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	if (FAILED(__super::Initialize(pArg))) //cameradesc
		return E_FAIL;

	DISABLE(this);

	return S_OK;
}

void CCamera_Kayoko::Tick(_float fTimeDelta)
{
	MoveCamera(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CCamera_Kayoko::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Kayoko::Render()
{
	return S_OK;
}

void CCamera_Kayoko::OnDisable()
{

}

void CCamera_Kayoko::OnEnable()
{

}

void CCamera_Kayoko::MoveCamera(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();

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

	_float fFov;
	fFov = XMConvertToRadians(fabs(XMVectorGetX(XMVector3Length((ParentMatrix.r[3] - ParentTargetMatrix.r[3])))) + 0.1f);

	m_CameraDesc.fFovy = fFov;

	XMStoreFloat4x4(&m_WorldMatrix, ParentMatrix * m_pTargetTransform->Get_WorldMatrix());
	XMStoreFloat4x4(&m_TargetWorldMatrix, ParentTargetMatrix * m_pTargetTransform->Get_WorldMatrix());

	//m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 1.f), XMConvertToRadians(-30.f));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4x4(&m_WorldMatrix).r[CTransform::STATE_TRANSLATION]);

	m_pTransformCom->LookAt(XMLoadFloat4x4(&m_TargetWorldMatrix).r[CTransform::STATE_TRANSLATION]);
}

CCamera_Kayoko * CCamera_Kayoko::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Kayoko*		pInstance = new CCamera_Kayoko(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Kayoko");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Kayoko::Clone(void * pArg)
{
	CCamera_Kayoko*		pInstance = new CCamera_Kayoko(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Kayoko");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Kayoko::Free()
{
	__super::Free();
}