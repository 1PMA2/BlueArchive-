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
}


CEx_Cutin::~CEx_Cutin()
{
}

void CEx_Cutin::Enter()
{
	

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
		CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student"), m_pOwner->Get_StudentInfo().eFormation);

		if (m_pOwner->Get_StudentInfo().eFormation == pStudent->Get_StudentInfo().eFormation)
		{
			if (ANIM_EXCUTIN == pStudent->Get_StudentInfo().eAnim)
			{
				pModel->Play_Animation(fTimeDelta);
				
				if (pModel->Get_isFinished())
				{
					pState = CEx_Cutin::Create(m_pOwner);
				}
			}
		}
	}


	else
	{
		pModel->Play_Animation(fTimeDelta);

		if (pModel->Get_isFinished())
		{
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