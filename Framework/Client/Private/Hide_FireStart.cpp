#include "stdafx.h"
#include "..\Public\Hide_FireStart.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Rifle_Fire.h"
#include "Run.h"
#include "Monster.h"

CHide_FireStart::CHide_FireStart(CStudent* pOwner)
	:CState(pOwner)
{
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));
		m_eAnim = ANIM_HIDEFIRESTART;
		pOwner->Set_State(m_eAnim);

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(19);
		break;
	case KAYOKO:
		pModel->Set_CurrentAnimation(18);
		break;

	}
}


void CHide_FireStart::Enter()
{

}

CState * CHide_FireStart::Loop(_float fTimeDelta)
{
	CState* pState = __super::Loop(fTimeDelta);

	if (nullptr != pState)
		return pState;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CMonster* pMonster = m_pOwner->FoundMonster();

	CTransform* pTTransform;
	
	if(nullptr != pMonster)
		pTTransform = (CTransform*)pMonster->Get_Component(TEXT("Com_Transform"));
	else
	{
		pState = CRun::Create(m_pOwner);
		return pState;
	}

	_vector		vTarget = pTTransform->Get_State(CTransform::STATE_TRANSLATION);
	
	pTransform->LookAtLerp(vTarget, 2.f, fTimeDelta);

	if (pModel->Get_isFinished())
	{ 
		pTransform->LookAt(pTTransform->Get_WorldMatrix().r[3]);
		
		switch (m_pOwner->Get_StudentInfo().eStudent)
		{
		case ARU:
			pState = CFire::Create(m_pOwner);
			break;
		case MUTSUKI:
			pState = CRifle_Fire::Create(m_pOwner);
			break;
		case KAYOKO:
			pState = CFire::Create(m_pOwner);
			break;
		}

	}

	return pState;
}

void CHide_FireStart::Exit()
{
	Destroy_Instance();
}

CHide_FireStart * CHide_FireStart::Create(CStudent* pOwner)
{
	return new CHide_FireStart(pOwner);
}