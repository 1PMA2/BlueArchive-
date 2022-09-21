#include "stdafx.h"
#include "..\Public\Formation_Pick.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Formation_Idle.h"
#include "Sensei.h"

CFormation_Pick::CFormation_Pick(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_FORMATIONPICK;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}

void CFormation_Pick::Enter()
{
}

CState * CFormation_Pick::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	if (m_pOwner->Is_Picked())
	{

		pState = CFormation_Pick::Create(m_pOwner);
	}
	else
	{
		pState = CFormation_Idle::Create(m_pOwner);
	}

	return pState;
}

void CFormation_Pick::Exit()
{
	Destroy_Instance();
}

CFormation_Pick * CFormation_Pick::Create(CStudent * pOwner)
{
	return new CFormation_Pick(pOwner);
}