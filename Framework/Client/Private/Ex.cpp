#include "stdafx.h"
#include "..\Public\Ex.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Fire.h"
#include "Knee_ZoomStart.h"

CEx::CEx(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_EX;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}

void CEx::Enter()
{

}

CState * CEx::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Play_Animation(fTimeDelta);

	if (pModel->Get_isFinished())
	{
		pState = CKnee_ZoomStart::Create(m_pOwner);
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