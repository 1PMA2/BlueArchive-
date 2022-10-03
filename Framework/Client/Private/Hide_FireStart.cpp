#include "stdafx.h"
#include "..\Public\Hide_FireStart.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Run.h"

CHide_FireStart::CHide_FireStart(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_HIDEFIRESTART;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}


void CHide_FireStart::Enter()
{

}

CState * CHide_FireStart::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTTransform;
	CGameObject* pMonster = pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), 0);

	if(nullptr != pMonster)
		pTTransform = (CTransform*)pMonster->Get_Component(TEXT("Com_Transform"));
	else
	{
		pState = CRun::Create(m_pOwner);
		return pState;
	}

	_vector		vTarget = pTTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector		vPosition = pTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector		vLook = vTarget - vPosition;

	_float		fAngle;

	fAngle = (acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(vLook),
		XMVector3Normalize(pTransform->Get_State(CTransform::STATE_LOOK))))));

	if (0 > XMVectorGetX(vLook))
		pTransform->TurnFor(XMVectorSet(0.f, -1.f, 0.f, 0.f), fTimeDelta, fAngle);
	else
		pTransform->TurnFor(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta, fAngle);
	

	if (pModel->Get_isFinished())
	{ 
		pTransform->LookAt(pTTransform->Get_WorldMatrix().r[3]);
		pState = CFire::Create(m_pOwner);
	}

	return pState;
}

void CHide_FireStart::Exit()
{
	Destroy_Instance();
}

CHide_FireStart * CHide_FireStart::Create(CStudent * pOwner)
{
	return new CHide_FireStart(pOwner);
}