#include "stdafx.h"
#include "..\Public\M_Run.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"
#include "M_Runend.h"
#include "Student.h"

CM_Run::CM_Run(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(9);
}

void CM_Run::Enter()
{

}

CMonster_State * CM_Run::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CStudent* pStudent = m_pOwner->FoundStudent();

	_vector vFear = pTransform->Get_State(CTransform::STATE_TRANSLATION);

	pModel->Repeat_Animation(fTimeDelta);

	pTransform->Go_Backward(fTimeDelta);

	if (m_pOwner->Get_Fear())
	{
		
		pTransform->LookAtLerp(XMVectorSet(XMVectorGetX(vFear), 0.f, XMVectorGetZ(vFear) - 1.f, 1.f), 5.f, fTimeDelta);
		return pState;
	}
	else
	{
		pTransform->LookAtLerp(XMVectorSet(XMVectorGetX(vFear), 0.f, XMVectorGetZ(vFear) + 1.f, 1.f), 5.f, fTimeDelta);
	}

	if (pStudent) //학생을 찾았음
	{

		pState = CM_Runend::Create(m_pOwner);
	}

		


	return pState;
}

void CM_Run::Exit()
{
	Destroy_Instance();
}

CM_Run * CM_Run::Create(CMonster * pOwner)
{
	return new CM_Run(pOwner);
}