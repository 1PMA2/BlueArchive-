#include "stdafx.h"
#include "..\Public\Knee_Reload.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Knee_ZoomStart.h"

CKnee_Reload::CKnee_Reload(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_KNEERELOAD;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}

void CKnee_Reload::Enter()
{
	m_pOwner->Reload();
}

CState * CKnee_Reload::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		pState = CKnee_ZoomStart::Create(m_pOwner);
	}

	return pState;
}

void CKnee_Reload::Exit()
{
	Destroy_Instance();
}

CKnee_Reload * CKnee_Reload::Create(CStudent * pOwner)
{
	return new CKnee_Reload(pOwner);
}