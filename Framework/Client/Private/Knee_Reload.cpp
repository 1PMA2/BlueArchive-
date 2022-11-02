#include "stdafx.h"
#include "..\Public\Knee_Reload.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Knee_ZoomStart.h"
#include "Sensei.h"

CKnee_Reload::CKnee_Reload(CStudent* pOwner)
	:CState(pOwner)
{
	
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));
		m_eAnim = ANIM_KNEERELOAD;
		pOwner->Set_State(m_eAnim);

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(14);
		break;
	case KAYOKO:
		pModel->Set_CurrentAnimation(7);
		break;
	case HARUKA:
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
		case HARUKA:
			CGameInstance::Get_Instance()->Play_Sound(L"SG_Reload",1, 0.8f);
			break;
		}
	}
}

void CKnee_Reload::Enter()
{
}

CState * CKnee_Reload::Loop(_float fTimeDelta)
{
	CState* pState = __super::Loop(fTimeDelta);

	if (nullptr != pState)
		return pState;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		m_pOwner->Reload();
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
