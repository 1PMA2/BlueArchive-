#include "stdafx.h"
#include "..\Public\Hide_Idle.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Hide_FireStart.h"

CHide_Idle::CHide_Idle(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_HIDEIDLE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}


CHide_Idle::~CHide_Idle()
{
}

void CHide_Idle::Enter()
{

}

CState * CHide_Idle::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;


	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	CTransform* pTTransform;
	pTTransform = (CTransform*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), 0)->Get_Component(TEXT("Com_Transform"));

	
	_vector		vTarget = pTTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector		vPosition = pTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector		vLook = vTarget - vPosition;

	_float		fAngle;

	fAngle = (acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(vLook),
		XMVector3Normalize(pTransform->Get_State(CTransform::STATE_LOOK))))));

	if (0 > XMVectorGetX(vLook))
		fAngle *= -1.f;

	_float		fDegree = XMConvertToDegrees(fAngle);


	if (5.f < fabs(fDegree))
	{
		if (0 < fDegree)
			pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
		else
			pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * -1.f);
	}
	else
	{
		pModel->Set_CurrentAnimation(ANIM_HIDEFIRESTART);
		pModel->Play_Animation(fTimeDelta);
		pState = CHide_FireStart::Create(m_pOwner);
	}



	

	

	return pState;
}

void CHide_Idle::Exit()
{
	Destroy_Instance();
}

CHide_Idle * CHide_Idle::Create(CStudent * pOwner)
{
	return new CHide_Idle(pOwner);
}
