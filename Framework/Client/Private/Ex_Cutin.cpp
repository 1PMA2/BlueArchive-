#include "stdafx.h"
#include "..\Public\Ex_Cutin.h"

#include "GameInstance.h"
#include "Student.h"
#include "Sensei.h"
#include "Fire.h"

CEx_Cutin::CEx_Cutin(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_EXCUTIN;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	if (true == m_pOwner->Get_StudentInfo().bExModel)
		pModel->Set_CurrentAnimation(0);
	else
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
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

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CSensei* pSensei = CSensei::Get_Instance();

	if (m_pOwner->Get_StudentInfo().bExModel)
	{
		CStudent* pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student"), 0);

		if (m_pOwner->Get_StudentInfo().eFormation == pStudent->Get_StudentInfo().eFormation)
		{
			if (ANIM_EXCUTIN == pStudent->Get_StudentInfo().eAnim)
			{
				pSensei->Use_Ex(true, m_pOwner->Get_StudentInfo().fExCost);

				if (pSensei->Get_SenseiInfo().bEx)
				{
					pModel->Play_Animation(fTimeDelta);
				}
				if (pModel->Get_isFinished())
				{
					pSensei->Use_Ex(false);
					
				}
			}
		}
	}

	else
	{
		pModel->Play_Animation(fTimeDelta);
		
		if (pModel->Get_isFinished())
			pState = CFire::Create(m_pOwner);
		
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