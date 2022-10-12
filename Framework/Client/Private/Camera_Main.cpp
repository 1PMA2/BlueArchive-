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

	return S_OK;
}

void CCamera_Main::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Move_Camera(fTimeDelta);

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

		vTarget = XMVectorSet(XMVectorGetX(vTarget), XMVectorGetY(vTarget), XMVectorGetZ(vTarget) - 4.f, 1.f); //offset

		_vector vLerp = XMVectorLerp(vCamera, vTarget, fTimeDelta * 1.f);

		_vector vMainCamera = XMVectorSet(XMVectorGetX(vCamera), XMVectorGetY(vCamera), XMVectorGetZ(vLerp), XMVectorGetW(vCamera));

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMainCamera);
	}

	Safe_Release(pGameInstance);
}

void CCamera_Main::Set_FovZ(_float fTimeDelta)
{
	if (2.3f < m_fFovRatio)//맨앞, 맨뒤 사이의 거리
	{
		_vector vFovy = XMVectorSet(0.f, 0.f, XMConvertToRadians(37.f), 1.f); //목표

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