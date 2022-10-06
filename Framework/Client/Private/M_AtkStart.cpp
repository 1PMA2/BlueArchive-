#include "stdafx.h"
#include "..\Public\M_AtkStart.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"
#include "M_Run.h"
#include "M_AtkIng.h"
#include "Student.h"

CM_AtkStart::CM_AtkStart(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(10);
}

void CM_AtkStart::Enter()
{

}

CMonster_State * CM_AtkStart::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CStudent* pStudent = m_pOwner->FoundStudent();

	CTransform* pTTransform;

	if (nullptr != pStudent)
		pTTransform = (CTransform*)pStudent->Get_Component(TEXT("Com_Transform"));
	else
	{
		pState = CM_Run::Create(m_pOwner);
		return pState;
	}

	_vector		vTarget = pTTransform->Get_State(CTransform::STATE_TRANSLATION);

	pTransform->LookAtLerp(vTarget, 5.f, -fTimeDelta);

	if (pModel->Get_isFinished())
	{
		pState = CM_AtkIng::Create(m_pOwner);

	}

	return pState;
}

void CM_AtkStart::Exit()
{
	Destroy_Instance();
}

CM_AtkStart * CM_AtkStart::Create(CMonster * pOwner)
{
	return new CM_AtkStart(pOwner);
}