#include "stdafx.h"
#include "..\Public\Ex.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Knee_ZoomStart.h"
#include "Knee_Reload.h"
#include "Sensei.h"
#include "Monster.h"
#include "Run.h"

CEx::CEx(CStudent* pOwner)
	:CState(pOwner)
{

	CGameInstance::Get_Instance()->SetChannelVolume(BGM, 0.5f);

	m_eAnim = ANIM_EX;
	pOwner->Set_State(m_eAnim);
	CSensei* pSensei = GET_SENSEI;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);

	pSensei->Use_Ex(false);

	m_bOnce = true;
	m_bMutsukli = true;

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		CGameInstance::Get_Instance()->Play_Sound_Rand(L"Aru_ExSkill_Level_", 3, 1.f);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		CGameInstance::Get_Instance()->Play_Sound_Rand(L"Mutsuki_ExSkill_Level_", 3, 1.f);
		break;
	case KAYOKO:
		pSensei->ReSet_Ex();
		pModel->Set_CurrentAnimation(33);
		CGameInstance::Get_Instance()->Play_Sound_Rand(L"Kayoko_ExSkill_Level_", 3, 1.f);
		for (_uint i = 0; i < pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Monster")); ++i)
		{
			CMonster* pMonster = (CMonster*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), i);

			pMonster->Set_Fear(true);
		}
		break;
	case HARUKA:
		pModel->Set_CurrentAnimation(32);
		CGameInstance::Get_Instance()->Play_Sound_Rand(L"Haruka_ExSkill_Level_", 3, 1.f);
		break;
	}
}

void CEx::Enter()
{


	
}

CState * CEx::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CSensei* pSensei = GET_SENSEI;

	pModel->Play_Animation(fTimeDelta);

	switch (m_pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		if (16 < pModel->Get_CurrentKeyFrame())
		{
			if (m_bOnce)
			{
				CGameInstance::Get_Instance()->Play_Sound(L"SFX_Aru_Skill_EX_2",1, 1.f);
				m_pMonster = pSensei->Get_LockonMonster();
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Cylinder"), &m_pOwner);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ExBullet"), TEXT("Prototype_GameObject_Aru_ExBullet"), &m_pMonster);
				m_bOnce = false;
			}

		}
		break;
	case MUTSUKI:
		if (m_bMutsukli)
		{
			m_pMonster = pSensei->Get_LockonMonster();
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ExBullet"), TEXT("Prototype_GameObject_Mutsuki_ExBullet"), &m_pMonster);
			m_bMutsukli = false;
		}
		break;
	case HARUKA:
		if (!m_b0 && m_b8) // 1.15, 2.05, 2.23, 3.04, 3.12, 3.19, 3.24, 4, 
			pTransform->Go_Straight(fTimeDelta * 0.2f);
		
		if (0.5f < pModel->Get_TimeAcc())
		{
			if (m_b0)
			{
				CGameInstance::Get_Instance()->Play_Sound(L"SFX_Skill_Haruka_Ex", 1, 1.f);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ExBullet"), TEXT("Prototype_GameObject_Haruka_ExBullet"), &m_pOwner);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Muzzle"), &m_pOwner);
				m_b0 = false;
			}
		}

		if (1.5f < pModel->Get_TimeAcc())
		{
			if (m_b1)
			{
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ExBullet"), TEXT("Prototype_GameObject_Haruka_ExBullet"), &m_pOwner);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Muzzle"), &m_pOwner);
				m_b1 = false;
			}
		}

		if (2.15f < pModel->Get_TimeAcc())
		{
			if (m_b2)
			{
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ExBullet"), TEXT("Prototype_GameObject_Haruka_ExBullet"), &m_pOwner);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Muzzle"), &m_pOwner);
				m_b2 = false;
			}
		}

		if (2.75f < pModel->Get_TimeAcc())
		{
			if (m_b3)
			{
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ExBullet"), TEXT("Prototype_GameObject_Haruka_ExBullet"), &m_pOwner);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Muzzle"), &m_pOwner);
				m_b3 = false;
			}
		}

		if (3.14f < pModel->Get_TimeAcc())
		{
			if (m_b4)
			{
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ExBullet"), TEXT("Prototype_GameObject_Haruka_ExBullet"), &m_pOwner);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Muzzle"), &m_pOwner);
				m_b4 = false;
			}
		}

		if (3.4f < pModel->Get_TimeAcc())
		{
			if (m_b5)
			{
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ExBullet"), TEXT("Prototype_GameObject_Haruka_ExBullet"), &m_pOwner);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Muzzle"), &m_pOwner);
				m_b5 = false;
			}
		}

		if (3.6f < pModel->Get_TimeAcc())
		{
			if (m_b6)
			{
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ExBullet"), TEXT("Prototype_GameObject_Haruka_ExBullet"), &m_pOwner);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Muzzle"), &m_pOwner);
				m_b6 = false;
			}
		}

		if (3.8f < pModel->Get_TimeAcc())
		{
			if (m_b7)
			{
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ExBullet"), TEXT("Prototype_GameObject_Haruka_ExBullet"), &m_pOwner);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Muzzle"), &m_pOwner);
				m_b7 = false;
			}
		}

		if (4.f < pModel->Get_TimeAcc())
		{
			if (m_b8)
			{
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ExBullet"), TEXT("Prototype_GameObject_Haruka_ExBullet"), &m_pOwner);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Muzzle"), &m_pOwner);
				m_b8 = false;
			}
		}
	break;
	}


	if (pModel->Get_isFinished())
	{ 

		if (0 < m_pOwner->Get_StudentInfo().iBullet)
			pState = CKnee_ZoomStart::Create(m_pOwner);
		else
			pState = CKnee_Reload::Create(m_pOwner);
	}

	return pState;
}

void CEx::Exit()
{
	Destroy_Instance();
}

CEx * CEx::Create(CStudent * pOwner)
{
	return new CEx(pOwner);
}