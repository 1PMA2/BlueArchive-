#include "stdafx.h"
#include "..\Public\Ex_Cutin.h"

#include "GameInstance.h"
#include "Student.h"
#include "Sensei.h"
#include "Fire.h"
#include "Ex.h"

CEx_Cutin::CEx_Cutin(CStudent* pOwner)
	:CState(pOwner)
{

	m_eAnim = ANIM_EXCUTIN;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	CGameInstance::Get_Instance()->SetChannelVolume(BGM, 0.1f);
	CGameInstance::Get_Instance()->WithoutBGM();
	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		CGameInstance::Get_Instance()->Play_Sound_Rand(L"Aru_ExSkill_", 3, 1.f);
		CGameInstance::Get_Instance()->Play_Sound(L"SFX_Aru_Skill_EX_1",1, 1.f);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		CGameInstance::Get_Instance()->Play_Sound_Rand(L"Mutsuki_ExSkill_", 3, 1.f);
		CGameInstance::Get_Instance()->Play_Sound(L"SFX_Skill_Mutsuki_Ex_Cut_in", 1, 1.f);
		break;
	case KAYOKO:
		pModel->Set_CurrentAnimation(6);
		break;
	case HARUKA:
		pModel->Set_CurrentAnimation(15);
		break;
	}
	

	pModel->ResetAnimation();
}


CEx_Cutin::~CEx_Cutin()
{
}

void CEx_Cutin::Enter()
{
	CSensei* pSensei = CSensei::Get_Instance();

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	_vector vTarget = pSensei->Get_LockonVector();

	pTransform->LookAt(vTarget);

}

CState * CEx_Cutin::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CSensei* pSensei = CSensei::Get_Instance();

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	
	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		pSensei->Set_ExStudent(nullptr);
		pSensei->Use_Ex(false);
		m_pOwner->Set_Ex(false);
		pState = CEx::Create(m_pOwner);
	}



	

	

	return pState;
}

void CEx_Cutin::Exit()
{
	Destroy_Instance();
}

CEx_Cutin * CEx_Cutin::Create(CStudent * pOwner)
{
	return new CEx_Cutin(pOwner);
}