#include "stdafx.h"
#include "..\Private\Run_ToHide.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Hide_Idle.h"
#include "Run.h"

CRun_ToHide::CRun_ToHide(CStudent* pOwner, CMonster* pTarget)
	:CState(pOwner, pTarget)
{
	m_eAnim = ANIM_RUNTOHIDE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);

	pModel->Set_OffSet(0.f, 0.f, -0.f);
}


CRun_ToHide::~CRun_ToHide()
{
}

void CRun_ToHide::Enter()
{
	
}

CState * CRun_ToHide::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	
	/*CTransform* pTTransform;
	pTTransform = (CTransform*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Obstacle"), 0)->Get_Component(TEXT("Com_Transform"));

	_float4 fOffset;

	XMStoreFloat4(&fOffset, pTTransform->Get_WorldMatrix().r[3]);

	_vector		vTarget;

	vTarget = XMVectorSet(fOffset.x, fOffset.y, fOffset.z - 0.7f, fOffset.w);

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));
	pTransform->Set_State(CTransform::STATE_TRANSLATION, vTarget);*/
	
	if (pModel->Get_isFinished())
	{
		pModel->Set_OffSet(0.f, 0.f, 0.f);
		pState = CHide_Idle::Create(m_pOwner);
	}
	

	return pState;
}

void CRun_ToHide::Exit()
{
	Destroy_Instance();
}

CRun_ToHide * CRun_ToHide::Create(CStudent * pOwner, CMonster* pTarget)
{
	return new CRun_ToHide(pOwner, pTarget);
}