#include "stdafx.h"
#include "..\Public\M_AtkEnd.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Model.h"
#include "M_AtkIng.h"
#include "Student.h"

CM_AtkEnd::CM_AtkEnd(CMonster* pOwner)
	:CMonster_State(pOwner)
{

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(7);
}

void CM_AtkEnd::Enter()
{

}

CMonster_State * CM_AtkEnd::Loop(_float fTimeDelta)
{
	CMonster_State* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		pState = CM_AtkIng::Create(m_pOwner);
	}

	return pState;
}

void CM_AtkEnd::Exit()
{
	Destroy_Instance();
}

CM_AtkEnd * CM_AtkEnd::Create(CMonster * pOwner)
{
	return new CM_AtkEnd(pOwner);
}