#include "stdafx.h"
#include "..\Public\Fire.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Run.h"

CFire::CFire(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_FIRE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}


CFire::~CFire()
{
}

void CFire::Enter()
{

}

CState * CFire::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	return pState;
}

void CFire::Exit()
{
	Destroy_Instance();
}

CFire * CFire::Create(CStudent * pOwner)
{
	return new CFire(pOwner);
}