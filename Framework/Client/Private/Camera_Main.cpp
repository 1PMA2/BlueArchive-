#include "stdafx.h"
#include "..\Public\Camera_Main.h"
#include "Student.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Student.h"

CCamera_Main::CCamera_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Main::CCamera_Main(const CCamera_Main & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Main::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Main::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_vMaxFov = XMVectorSet(0.f, 0.f, m_CameraDesc.fFovy, 1.f);
	m_vCurrentFov = XMVectorSet(0.f, 0.f, m_CameraDesc.fFovy, 1.f);

	m_bShake = false;
	return S_OK;
}

void CCamera_Main::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	

	if (KEY(D, HOLD))
	{
		_float fZposition = XMVectorGetZ(m_pTransformCom->Get_WorldMatrix().r[3]);
		fZposition += fTimeDelta * 5.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_CameraDesc.vEye.x, m_CameraDesc.vEye.y, fZposition, 1.f));

		if (KEY(S, HOLD))
		{
			Set_Fov(XMConvertToRadians(25.f));
		}
	} //map 확인용
	else
	{
		Shake_Camera(fTimeDelta);

		Move_Camera(fTimeDelta);
	}

	if (FAILED(Bind_PipeLine()))
		return;
}

void CCamera_Main::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Main::Render()
{
	return S_OK;
}

void CCamera_Main::OnDisable()
{
}

void CCamera_Main::OnEnable()
{
}

void CCamera_Main::Shake_Camera(_float fTimeDelta)
{
	if (m_bShake)
	{
		m_fShakeTime += fTimeDelta;

		_float4 vTranslation;

		XMStoreFloat4(&vTranslation, m_vMainCamera);

		_float fShakeY = frandom(-0.05f, 0.05f);
		_float fShakeZ = frandom(-0.05f, 0.05f);

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(vTranslation.x, vTranslation.y + fShakeY, vTranslation.z + fShakeZ, 1.f));
	}

	if (0.2f < m_fShakeTime)
	{		
		m_bShake = false;
		m_fShakeTime = 0.f;
	}
}

void CCamera_Main::Move_Camera(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (_uint i = 0; i < pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Student")); ++i)
	{
		CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student"), i);

		CTransform* pTransform = (CTransform*)pStudent->Get_Component(TEXT("Com_Transform"));

		_vector vTarget = pTransform->Get_State(CTransform::STATE_TRANSLATION);

		_float fZLength = XMVectorGetZ(vTarget);

		if (m_fMax < fZLength)
		{
			m_fMax = fZLength;
			m_pStudent = pStudent;
		}

		if (m_fMin > fZLength)
		{
			m_fMin = fZLength;
		}
	}

	m_fFovRatio = fabs(m_fMax - m_fMin);



	m_fMax = 0.f;
	m_fMin = 999.f;

	if (nullptr != m_pStudent)
	{
		Set_FovZ(fTimeDelta);

		CTransform* pTransform = (CTransform*)m_pStudent->Get_Component(TEXT("Com_Transform"));

		_vector vCamera = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

		_vector vTarget = pTransform->Get_State(CTransform::STATE_TRANSLATION); //학생을 봄

		m_vTarget = XMVectorSet(XMVectorGetX(vTarget), XMVectorGetY(vTarget), XMVectorGetZ(vTarget) - 5.f, 1.f); //offset

		_vector vLerp = XMVectorLerp(vCamera, m_vTarget, fTimeDelta * 1.f);

		_vector vLerpXY = XMVectorLerp(vCamera, XMLoadFloat4(&m_CameraDesc.vEye), fTimeDelta);

		m_vMainCamera = XMVectorSet(XMVectorGetX(vLerpXY), XMVectorGetY(vLerpXY), XMVectorGetZ(vLerp), 1.f);

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_vMainCamera);
	}

	Safe_Release(pGameInstance);
}

void CCamera_Main::Set_FovZ(_float fTimeDelta)
{
	if (2.4f < m_fFovRatio)//맨앞, 맨뒤 사이의 거리
	{
		_vector vFovy = XMVectorSet(0.f, 0.f, XMConvertToRadians(25.f), 1.f); //목표

		m_vCurrentFov = XMVectorLerp(m_vCurrentFov, vFovy, fTimeDelta * 0.7f);
	}
	else
	{
		m_vCurrentFov = XMVectorLerp(m_vCurrentFov, m_vMaxFov, fTimeDelta * 0.7f);
	}

	Set_Fov((XMVectorGetZ(m_vCurrentFov)));
}

CCamera_Main * CCamera_Main::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Main*		pInstance = new CCamera_Main(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Main");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Main::Clone(void * pArg)
{
	CCamera_Main*		pInstance = new CCamera_Main(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Main");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Main::Free()
{
	__super::Free();
}