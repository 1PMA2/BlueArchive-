#include "stdafx.h"
#include "..\Private\Run_ToHide.h"

#include "GameInstance.h"
#include "Student.h"
#include "Model.h"
#include "Hide_Idle.h"

CRun_ToHide::CRun_ToHide(CStudent* pOwner)
	:CState(pOwner)
{
	m_eAnim = ANIM_RUNTOHIDE;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);

	
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


	pModel->Play_Animation(fTimeDelta);


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTTransform;
	pTTransform = (CTransform*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Obstacle"), 0)->Get_Component(TEXT("Com_Transform"));

	_float4 f;

	XMStoreFloat4(&f, pTTransform->Get_WorldMatrix().r[3]);

	_vector		vTarget;

	vTarget = XMVectorSet(f.x, f.y, f.z - 0.7f, f.w);

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));
	pTransform->Set_State(CTransform::STATE_TRANSLATION, vTarget);

	if (pModel->Get_isFinished())
	{
		pState = CHide_Idle::Create(m_pOwner);
	}
	

	return pState;
}

void CRun_ToHide::Exit()
{
	Destroy_Instance();
}

CRun_ToHide * CRun_ToHide::Create(CStudent * pOwner)
{
	return new CRun_ToHide(pOwner);
}