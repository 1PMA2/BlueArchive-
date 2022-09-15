#include "stdafx.h"
#include "..\Public\Run.h"
#include "GameInstance.h"
#include "Student.h"
#include "Sensei.h"

CRun::CRun(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_RUN;

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	if(true == m_pOwner->Get_StudentInfo().bExModel)
		pModel->Set_CurrentAnimation(m_pOwner->Get_StudentInfo().eAnim);
	else
		pModel->Set_CurrentAnimation(m_eAnim);
}


CRun::~CRun()
{
}

void CRun::Enter()
{

}

CState * CRun::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CSensei* pSensei = CSensei::Get_Instance();

	if (false == m_pOwner->Get_StudentInfo().bExModel)
	{
		pTransform->Go_Straight(fTimeDelta);
		pModel->Play_Animation(fTimeDelta);
	}

	else
	{
		if (GetKeyState(VK_UP) & 0x8000)
		{
			pSensei->Use_Ex(true);
		}
		if (pSensei->Get_SenseiInfo().bEx)
		{
			pModel->Play_Animation(fTimeDelta);
		}
		if (pModel->Get_isFinished())
		{
			pSensei->Use_Ex(false);
		}
	}



	return pState;
}

void CRun::Exit()
{
	Destroy_Instance();
}

CRun * CRun::Create(CStudent * pOwner)
{
	return new CRun(pOwner);
}