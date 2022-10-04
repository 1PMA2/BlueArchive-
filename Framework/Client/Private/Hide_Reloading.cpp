#include "stdafx.h"
#include "..\Public\Hide_Reloading.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Hide_FireStart.h"

CHide_Reloading::CHide_Reloading(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_HIDERELOADING;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}

void CHide_Reloading::Enter()
{
	m_pOwner->Reload();
}

CState * CHide_Reloading::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		pState = CHide_FireStart::Create(m_pOwner, m_pTarget, m_pCover);
	}

	return pState;
}

void CHide_Reloading::Exit()
{
	Destroy_Instance();
}

CHide_Reloading * CHide_Reloading::Create(CStudent * pOwner)
{
	return new CHide_Reloading(pOwner);
}