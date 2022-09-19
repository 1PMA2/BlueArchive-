#include "stdafx.h"
#include "..\Public\Knee_ZoomStart.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Knee_ZoomFire.h"

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

	if (pModel->Get_isFinished())
	{
		pState = CKnee_ZoomFire::Create(m_pOwner);
	}

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