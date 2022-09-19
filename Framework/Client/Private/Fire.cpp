#include "stdafx.h"
#include "..\Public\Fire.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Ex_Cutin.h"
#include "Hide_ReloadStart.h"

CFire::CFire(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_FIRE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}


void CFire::Enter()
{
	m_pOwner->Use_Bullet();
}

CState * CFire::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	if (pModel->Get_isFinished())
	{
		if(0 < m_pOwner->Get_StudentInfo().iBullet)
			pState = CFire::Create(m_pOwner);
		else
			pState = CHide_ReloadStart::Create(m_pOwner);

		return pState;
	}

	if (GetKeyState(VK_SPACE) & 0x8000)
		pState = CEx_Cutin::Create(m_pOwner);

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