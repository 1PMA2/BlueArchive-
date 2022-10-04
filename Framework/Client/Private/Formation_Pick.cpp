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

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Repeat_Animation(fTimeDelta);

	_float4 fOut;

	pGameInstance->Picking((CVIBuffer*)pGameInstance->Get_Component(LEVEL_FORMATION, TEXT("Layer_Formation_BackGround"), TEXT("Com_VIBuffer"), 0),
		(CTransform*)pGameInstance->Get_Component(LEVEL_FORMATION, TEXT("Layer_Formation_BackGround"), TEXT("Com_Transform"), 0), &fOut);

	fOut.y -= 0.5f; // offset
	pTransform->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&fOut));

	if (-0.5f < fOut.y && 1.f > fOut.y)
	{
		if (1.f <= fOut.x)
		{
			m_pOwner->Set_Formation(FORMATION_FIRST);
			m_vPreTranslation = XMVectorSet(1.5f, 0.f, 0.f, 1.f);
		}
		else if (1.f > fOut.x && 0.f <= fOut.x)
		{
			m_pOwner->Set_Formation(FORMATION_SECOND);
			m_vPreTranslation = XMVectorSet(0.5f, 0.f, 0.f, 1.f);
		}
		else if (0.f > fOut.x && -1.f <= fOut.x)
		{
			m_pOwner->Set_Formation(FORMATION_THIRD);
			m_vPreTranslation = XMVectorSet(-0.5f, 0.f, 0.f, 1.f);
		}
		else if (-1.f > fOut.x)
		{
			m_pOwner->Set_Formation(FORMATION_FOURTH);
			m_vPreTranslation = XMVectorSet(-1.5f, 0.f, 0.f, 1.f);
		}
	}  //포메이션 위치
	else
	{
		FORMATION eFormation = m_pOwner->Get_StudentInfo().eFormation;
		
		switch (eFormation)
		{
		case FORMATION_FIRST:
			m_vPreTranslation = XMVectorSet(1.5f, 0.f, 0.f, 1.f);
			break;
		case FORMATION_SECOND:
			m_vPreTranslation = XMVectorSet(0.5f, 0.f, 0.f, 1.f);
			break;
		case FORMATION_THIRD:
			m_vPreTranslation = XMVectorSet(-0.5f, 0.f, 0.f, 1.f);
			break;
		case FORMATION_FOURTH:
			m_vPreTranslation = XMVectorSet(-1.5f, 0.f, 0.f, 1.f);
			break;
		}
	}

	if (KEY(LBUTTON, HOLD))
	{
		pState = CFormation_Pick::Create(m_pOwner);
	}
	else
	{
		pTransform->Set_State(CTransform::STATE_TRANSLATION, m_vPreTranslation);
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