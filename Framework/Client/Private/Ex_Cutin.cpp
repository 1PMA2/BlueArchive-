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

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CSensei* pSensei = CSensei::Get_Instance();

	if (true == m_pOwner->Get_StudentInfo().bExModel)
		pModel->Set_CurrentAnimation(0);
	else
	{
		pSensei->Use_Ex(true, m_pOwner->Get_StudentInfo().fExCost);
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
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

	if (m_pOwner->Get_StudentInfo().bExModel)
	{
		for (_uint i = 0; i < pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Student")); ++i)
		{
			CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student"), i);

			if (pStudent->Get_Ex())
			{
				pSensei->Use_Ex(true, m_pOwner->Get_StudentInfo().fExCost);
				pModel->Play_Animation(fTimeDelta);
				break;
			}
			else
			{
				pModel->ResetAnimation();
			}
		}
	}
	else
	{
		pModel->Play_Animation(fTimeDelta);

		if (pModel->Get_isFinished())
		{
			pSensei->Use_Ex(false);
			m_pOwner->Set_Ex(false);
			pState = CEx::Create(m_pOwner);
		}
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