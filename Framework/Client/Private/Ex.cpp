#include "stdafx.h"
#include "..\Public\Ex.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Knee_ZoomStart.h"
#include "Knee_Reload.h"
#include "Sensei.h"
#include "Run.h"

CEx::CEx(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_EX;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case ARU:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case MUTSUKI:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		break;
	case KAYOKO:
		pModel->Set_CurrentAnimation(33);
		break;

	}
}

void CEx::Enter()
{
	CSensei* pSensei = GET_SENSEI;

	pSensei->Use_Ex(false);
}

CState * CEx::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{ 


		if (0 < m_pOwner->Get_StudentInfo().iBullet)
			pState = CKnee_ZoomStart::Create(m_pOwner);
		else
			pState = CKnee_Reload::Create(m_pOwner);
	}

	return pState;
}

void CEx::Exit()
{
	Destroy_Instance();
}

CEx * CEx::Create(CStudent * pOwner)
{
	return new CEx(pOwner);
}