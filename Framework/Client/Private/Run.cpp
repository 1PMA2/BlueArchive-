#include "stdafx.h"
#include "..\Public\Run.h"
#include "GameInstance.h"
#include "Student.h"
#include "Sensei.h"
#include "Run_Sign.h"
#include "Run_ToHide.h"
#include "Ex_Cutin.h"

CRun::CRun(CStudent* pOwner)
	:CState(pOwner)
{
	if (TEXT("Aru") == pOwner->Get_StudentInfo().pName)
	{
		int i = 10;
	}

	m_eAnim = ANIM_RUN;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
}


CRun::~CRun()
{
}

void CRun::Enter()
{

}

CState * CRun::Loop(_float fTimeDelta)
{
	CState* pState = nullptr;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));
	CSensei* pSensei = CSensei::Get_Instance();

	if (m_pOwner->FoundMonster())
	{
		if (m_pOwner->FoundObstacle())
		{
			CTransform* pTTransform;
			pTTransform = (CTransform*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Obstacle"), 0)->Get_Component(TEXT("Com_Transform"));

			_float4 f;

			XMStoreFloat4(&f, pTTransform->Get_WorldMatrix().r[3]);

			_vector		vTarget;

			vTarget = XMVectorSet(f.x, f.y, f.z - 0.7f, f.w);

			_vector		vPosition = pTransform->Get_State(CTransform::STATE_TRANSLATION);

			_vector		vLook = vTarget - vPosition;

			_float		fAngle;

			fAngle = (acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(vLook),
				XMVector3Normalize(pTransform->Get_State(CTransform::STATE_LOOK))))));

			if (0 > XMVectorGetX(vLook))
				pTransform->TurnFor(XMVectorSet(0.f, -1.f, 0.f, 0.f), fTimeDelta, fAngle);
			else
				pTransform->TurnFor(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta, fAngle);
	



			if (0.35f < XMVectorGetX(XMVector3Length(vLook)))
				pTransform->Chase(vTarget, fTimeDelta);
			else
			{
				pState = CRun_ToHide::Create(m_pOwner);
			}
		}
		
	}

	else
	{
		pTransform->Go_Straight(fTimeDelta);
	}

	if (pModel->Get_isFinished())
		pState = CRun::Create(m_pOwner);

	return pState;
}

void CRun::Exit()
{
	Destroy_Instance();
}

void CRun::Find_Monster()
{
}

CRun * CRun::Create(CStudent * pOwner)
{
	return new CRun(pOwner);
}