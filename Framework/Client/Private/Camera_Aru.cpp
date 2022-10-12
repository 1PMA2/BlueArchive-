#include "stdafx.h"
#include "..\Public\Camera_Aru.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Sensei.h"
#include "Student.h"

CCamera_Aru::CCamera_Aru(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera_Ex(pDevice, pContext)
{
}

CCamera_Aru::CCamera_Aru(const CCamera_Aru & rhs)
	: CCamera_Ex(rhs)
{
}

HRESULT CCamera_Aru::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Aru::Initialize(void * pArg)
{

	if (nullptr != pArg)
		m_pAruEx = (CStudent*)pArg;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CModel*		pTargetModel = (CModel*)m_pAruEx->Get_Component(TEXT("Com_Model"));

	m_pBonePtr = pTargetModel->Find_HierarcyNode("Camera001");
	m_pTargetBonePtr = pTargetModel->Find_HierarcyNode("Camera001.Target");

	m_pTargetTransform = (CTransform*)m_pAruEx->Get_Component(TEXT("Com_Transform"));


	RELEASE_INSTANCE(CGameInstance);

	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	if (FAILED(__super::Initialize(pArg))) //cameradesc
		return E_FAIL;

	DISABLE(this);

	return S_OK;
}

void CCamera_Aru::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	MoveCamera(fTimeDelta);
	
	__super::Tick(fTimeDelta);
}

void CCamera_Aru::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Aru::Render()
{
	return S_OK;
}

void CCamera_Aru::OnDisable()
{

}

void CCamera_Aru::OnEnable()
{

	
}

void CCamera_Aru::MoveCamera(_float fTimeDelta)
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
	fFov = XMConvertToRadians(fabs(XMVectorGetX(XMVector3Length((ParentMatrix.r[3] - ParentTargetMatrix.r[3])))) + 20.f);

	m_CameraDesc.fFovy = fFov * 0.85f;

	XMStoreFloat4x4(&m_WorldMatrix, ParentMatrix * m_pTargetTransform->Get_WorldMatrix());
	XMStoreFloat4x4(&m_TargetWorldMatrix, ParentTargetMatrix * m_pTargetTransform->Get_WorldMatrix());

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4x4(&m_WorldMatrix).r[CTransform::STATE_TRANSLATION]);

	m_pTransformCom->LookAt(XMLoadFloat4x4(&m_TargetWorldMatrix).r[CTransform::STATE_TRANSLATION]);

	//m_pTransformCom->LookAtLerp(XMLoadFloat4x4(&m_TargetWorldMatrix).r[CTransform::STATE_TRANSLATION], 3.f, fTimeDelta);
}

CCamera_Aru * CCamera_Aru::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Aru*		pInstance = new CCamera_Aru(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Aru");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Aru::Clone(void * pArg)
{
	CCamera_Aru*		pInstance = new CCamera_Aru(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Aru");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Aru::Free()
{
	__super::Free();
}