#include "stdafx.h"
#include "..\Public\Knee_ZoomStart.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Knee_ZoomFire.h"
#include "Monster.h"
#include "Run.h"

CKnee_ZoomStart::CKnee_ZoomStart(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_KNEEZOOMSTART;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}

void CKnee_ZoomStart::Enter()
{

}

CState * CKnee_ZoomStart::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTTransform;

	if (nullptr != m_pOwner->FoundMonster())
		pTTransform = (CTransform*)m_pOwner->FoundMonster()->Get_Component(TEXT("Com_Transform"));
	else
	{
		pState = CRun::Create(m_pOwner);
		return pState;
	}

	_vector		vTarget = pTTransform->Get_State(CTransform::STATE_TRANSLATION);

	pTransform->LookAtLerp(vTarget, 2.f, fTimeDelta);

	if (pModel->Get_isFinished())
	{
		pTransform->LookAt(pTTransform->Get_WorldMatrix().r[3]);
		pState = CKnee_ZoomFire::Create(m_pOwner);
	}

	return pState;

	return pState;
}

void CKnee_ZoomStart::Exit()
{
	Destroy_Instance();
}

CKnee_ZoomStart * CKnee_ZoomStart::Create(CStudent * pOwner)
{
	return new CKnee_ZoomStart(pOwner);
}