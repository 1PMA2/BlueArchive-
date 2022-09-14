#include "stdafx.h"
#include "..\Public\Run.h"
#include "Transform.h"
#include "Model.h"
#include "Student.h"

CRun::CRun(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_RUN;

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

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

	pTransform->Go_Straight(fTimeDelta);


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