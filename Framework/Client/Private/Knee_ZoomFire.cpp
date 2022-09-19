#include "stdafx.h"
#include "..\Public\Knee_ZoomFire.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Knee_ZoomEnd.h"
#include "Ex_Cutin.h"

CKnee_ZoomFire::CKnee_ZoomFire(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_KNEEZOOMFIRE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}

void CKnee_ZoomFire::Enter()
{
	m_pOwner->Use_Bullet();
}

CState * CKnee_ZoomFire::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	//pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		if (0 < m_pOwner->Get_StudentInfo().iBullet)
			pState = CKnee_ZoomFire::Create(m_pOwner);
		else
		{
			pState = CKnee_ZoomEnd::Create(m_pOwner);
		}
		return pState;
	}

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		pState = CEx_Cutin::Create(m_pOwner);
	}

	return pState;
}

void CKnee_ZoomFire::Exit()
{
	Destroy_Instance();
}

CKnee_ZoomFire * CKnee_ZoomFire::Create(CStudent * pOwner)
{
	return new CKnee_ZoomFire(pOwner);
}