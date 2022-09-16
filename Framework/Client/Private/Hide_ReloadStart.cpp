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
	m_eAnim = ANIM_HIDERELOADSTART;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
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
		pModel->Set_CurrentAnimation(ANIM_HIDERELOADING);
		pModel->Play_Animation(fTimeDelta);
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