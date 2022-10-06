#include "stdafx.h"
#include "..\Public\Knee_ZoomEnd.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Knee_Reload.h"

CKnee_ZoomEnd::CKnee_ZoomEnd(CStudent* pOwner)
	:CState(pOwner)
{
	


	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		m_eAnim = ANIM_KNEEZOOMEND;
		pOwner->Set_State(m_eAnim);
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(9);
		break;
	case 2:
		break;

	}
}

void CKnee_ZoomEnd::Enter()
{

}

CState * CKnee_ZoomEnd::Loop(_float fTimeDelta)
{
	CState* pState = __super::Loop(fTimeDelta);

	if (nullptr != pState)
		return pState;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		pState = CKnee_Reload::Create(m_pOwner);
	}

	return pState;
}

void CKnee_ZoomEnd::Exit()
{
	Destroy_Instance();
}

CKnee_ZoomEnd * CKnee_ZoomEnd::Create(CStudent * pOwner)
{
	return new CKnee_ZoomEnd(pOwner);
}