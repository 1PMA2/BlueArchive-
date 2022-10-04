#include "stdafx.h"
#include "..\Public\Hide_FireStart.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Run.h"
#include "Monster.h"

CHide_FireStart::CHide_FireStart(CStudent* pOwner, CMonster* pTarget, CForkLift* pCover)
	:CState(pOwner, pTarget, pCover)
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

	pModel->Play_Animation(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTTransform;

	if(nullptr != m_pOwner->FoundMonster())
		pTTransform = (CTransform*)m_pOwner->FoundMonster()->Get_Component(TEXT("Com_Transform"));
	else
	{
		pState = CRun::Create(m_pOwner);
		return pState;
	}

	_vector		vTarget = pTTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector		vPosition = pTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector		vMyLook = pTransform->Get_State(CTransform::STATE_LOOK);

	_vector		vLook = vTarget - vPosition;

	_vector		vLerpLook = XMVectorLerp(vMyLook, vLook, fTimeDelta * 2.f);

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLerpLook);

	_vector		vUp = XMVector3Cross(vLerpLook, vRight);


	pTransform->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));
	pTransform->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));
	pTransform->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLerpLook));


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

CHide_FireStart * CHide_FireStart::Create(CStudent* pOwner, CMonster* pTarget, CForkLift* pCover)
{
	return new CHide_FireStart(pOwner, pTarget, pCover);
}