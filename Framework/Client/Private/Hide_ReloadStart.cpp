#include "stdafx.h"
#include "..\Public\Hide_ReloadStart.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Hide_Reloading.h"

CHide_ReloadStart::CHide_ReloadStart(CStudent* pOwner)
	:CState(pOwner)
{
	
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	switch (pOwner->Get_StudentInfo().iIndex)
	{
	case 0:
		m_eAnim = ANIM_HIDERELOADSTART;
		pOwner->Set_State(m_eAnim);
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case 1:
		pModel->Set_CurrentAnimation(17);
		break;
	case 2:
		break;
	}
}

void CHide_ReloadStart::Enter()
{

}

CState * CHide_ReloadStart::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		pState = CHide_Reloading::Create(m_pOwner);
	}

	return pState;
}

void CHide_ReloadStart::Exit()
{
	Destroy_Instance();
}

CHide_ReloadStart * CHide_ReloadStart::Create(CStudent * pOwner)
{
	return new CHide_ReloadStart(pOwner);
}