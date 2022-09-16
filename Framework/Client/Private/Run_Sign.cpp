#include "stdafx.h"
#include "..\Public\Run_Sign.h"
#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Run.h"

CRun_Sign::CRun_Sign(CStudent* pOwner)
	:CState(pOwner)
{	
	m_eAnim = ANIM_RUNSIGN;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}


CRun_Sign::~CRun_Sign()
{
}

void CRun_Sign::Enter()
{

}

CState * CRun_Sign::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;
	
	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));
	
	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pTransform->Go_Straight(fTimeDelta);

	pModel->Play_Animation(fTimeDelta);
	
	if (pModel->Get_isFinished())
	{
		pModel->Set_CurrentAnimation(ANIM_RUN);
		pModel->Play_Animation(fTimeDelta);

		pState = CRun::Create(m_pOwner);
	}

	return pState;
}

void CRun_Sign::Exit()
{
	Destroy_Instance();
}

CRun_Sign * CRun_Sign::Create(CStudent * pOwner)
{
	return new CRun_Sign(pOwner);
}
