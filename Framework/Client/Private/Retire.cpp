#include "stdafx.h"
#include "..\Public\Retire.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Sensei.h"

CRetire::CRetire(CStudent* pOwner)
	:CState(pOwner)
{
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));
	m_eAnim = ANIM_RETIRE;
	pOwner->Set_State(m_eAnim);

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(25);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(25);
		break;
	case KAYOKO:
		pModel->Set_CurrentAnimation(28);
		break;
	case HARUKA:
		pModel->Set_CurrentAnimation(26);
		break;
	}
	
	m_pOwner->Set_Retire(true);
}

void CRetire::Enter()
{

}

CState * CRetire::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CSensei* pSensei = GET_SENSEI;

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		if (m_bOnce)
		{
			pSensei->Retirecount();
			m_bOnce = false;
		}
		DISABLE(m_pOwner);
	}

	return pState;
}

void CRetire::Exit()
{
	Destroy_Instance();
}

CRetire * CRetire::Create(CStudent * pOwner)
{
	return new CRetire(pOwner);
}