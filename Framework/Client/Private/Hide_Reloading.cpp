#include "stdafx.h"
#include "..\Public\Hide_Reloading.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Hide_FireStart.h"
#include "Sensei.h"

CHide_Reloading::CHide_Reloading(CStudent* pOwner)
	:CState(pOwner)
{
	
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));
		m_eAnim = ANIM_HIDERELOADING;
		pOwner->Set_State(m_eAnim);

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(22);
		break;
	case KAYOKO:
		pModel->Set_CurrentAnimation(12);
		break;
	}

	CSensei* pSensei = CSensei::Get_Instance();

	if (!pSensei->Get_SenseiInfo().bEx)
	{
		switch (m_pOwner->Get_StudentInfo().eStudent)
		{
		case ARU:
			CGameInstance::Get_Instance()->Play_Sound(L"SR_Reload",1, 0.8f);
			break;
		case MUTSUKI:
			CGameInstance::Get_Instance()->Play_Sound(L"AR_Reload",1, 0.8f);
			break;
		case KAYOKO:
			CGameInstance::Get_Instance()->Play_Sound(L"HG_Reload",1, 0.8f);
			break;
		}
	}
}

void CHide_Reloading::Enter()
{
	m_pOwner->Reload();
}

CState * CHide_Reloading::Loop(_float fTimeDelta)
{
	CState* pState = __super::Loop(fTimeDelta);

	if (nullptr != pState)
		return pState;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		pState = CHide_FireStart::Create(m_pOwner);
	}

	return pState;
}

void CHide_Reloading::Exit()
{
	Destroy_Instance();
}

CHide_Reloading * CHide_Reloading::Create(CStudent* pOwner)
{
	return new CHide_Reloading(pOwner);
}