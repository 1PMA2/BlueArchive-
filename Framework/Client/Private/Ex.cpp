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
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case KAYOKO:
		pSensei->ReSet_Ex();
		pModel->Set_CurrentAnimation(33);
		for (_uint i = 0; i < pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Monster")); ++i)
		{
			CMonster* pMonster = (CMonster*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), i);

			pMonster->Set_Fear(true);
		}
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
	default:
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