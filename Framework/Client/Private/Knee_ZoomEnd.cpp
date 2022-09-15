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
	m_eAnim = ANIM_KNEEZOOMEND;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}

void CKnee_ZoomEnd::Enter()
{

}

CState * CKnee_ZoomEnd::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

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