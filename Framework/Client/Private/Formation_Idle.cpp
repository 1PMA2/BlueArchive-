#include "stdafx.h"
#include "..\Public\Formation_Idle.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Formation_Pick.h"
#include "Sensei.h"

CFormation_Idle::CFormation_Idle(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_FORMATIONIDLE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}

void CFormation_Idle::Enter()
{
}

CState * CFormation_Idle::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CCollider* pAABBcom = (CCollider*)m_pOwner->Get_Component(TEXT("Com_AABB"));

	if (pAABBcom->CollisionRay())
	{
		if (KEY(LBUTTON, TAP))
		{
			pState = CFormation_Pick::Create(m_pOwner);
			return pState;
		}
	}

	pState = CFormation_Idle::Create(m_pOwner);

	return pState;
}

void CFormation_Idle::Exit()
{
	Destroy_Instance();
}

CFormation_Idle * CFormation_Idle::Create(CStudent * pOwner)
{
	return new CFormation_Idle(pOwner);
}