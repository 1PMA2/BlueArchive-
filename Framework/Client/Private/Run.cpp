#include "stdafx.h"
#include "..\Public\Run.h"
#include "GameInstance.h"
#include "Student.h"
#include "Sensei.h"
#include "Run_Sign.h"
#include "Run_ToHide.h"
#include "Ex_Cutin.h"
#include "Collider.h"
#include "Monster.h"
#include "ForkLift.h"

CRun::CRun(CStudent* pOwner)
	:CState(pOwner)
{
	if (TEXT("Aru") == pOwner->Get_StudentInfo().pName)
	{
		int i = 10;
	}

	m_eAnim = ANIM_RUN;
	pOwner->Set_State(m_eAnim);

	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));
	
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

	pModel->Repeat_Animation(fTimeDelta);

	pState = Find_Monster(fTimeDelta);

	return pState;
}

void CRun::Exit()
{
	Destroy_Instance();
}

CState* CRun::Find_Monster(_float fTimeDelta)
{
	m_pOwner->Reset_Monsters();

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();

	CTransform* pStudentTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	_uint iMonsterCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

	_vector vTranslation = pStudentTransform->Get_State(CTransform::STATE_TRANSLATION);

	if (0 >= iMonsterCount)
	{
		pStudentTransform->Go_Straight(fTimeDelta);
		_vector vXY = pStudentTransform->Get_WorldMatrix().r[3];
		pStudentTransform->LookAtLerp(XMVectorSet(XMVectorGetX(vXY), XMVectorGetY(vXY), 65.f, 1.f), 1.f, fTimeDelta);
		return nullptr;
	}

	for (_uint i = 0; i < iMonsterCount; ++i)
	{
		CMonster* pMonster = (CMonster*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), i);

		CTransform* pMonsterTransform = (CTransform*)pMonster->Get_Component(TEXT("Com_Transform"));

		_vector vMonsterTranslation = pMonsterTransform->Get_State(CTransform::STATE_TRANSLATION);

		_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vTranslation)); //학생과 모든 몬스터 사이의 거리 

		if ((_float)m_pOwner->Get_StudentInfo().iRange > fLength) //레이어의 몬스터 검사 후 범위 내 몬스터 
		{
			if (m_fMin > fLength)
			{
				m_fMin = fLength;
				m_pTargetMonster = pMonster; //가장 가까운 몬스터
			}
			m_pOwner->In_RangeMonsters(pMonster);
		}
		else
			m_pTargetMonster = nullptr;
	}

	if (nullptr != m_pTargetMonster)
	{

		//장애물 검사 > 범위 안에 장애물이 있으며 장애물과 가장 가까운몬스터의 거리가 irange 이내일 경우 장애물로 이동함

		_uint iCoverCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Cover"));

		//if (0 >= iCoverCount)
		//	return nullptr; // run to knee

		for (_uint i = 0; i < iCoverCount; ++i)
		{
			CForkLift* pCover = (CForkLift*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cover"), i);

			CTransform* pCoverTransform = (CTransform*)pCover->Get_Component(TEXT("Com_Transform"));

			CTransform* pMonsterTransform = (CTransform*)m_pTargetMonster->Get_Component(TEXT("Com_Transform"));

			_vector vCoverTranslation = pCoverTransform->Get_State(CTransform::STATE_TRANSLATION); //엄폐물
			_vector vMonsterTranslation = pMonsterTransform->Get_State(CTransform::STATE_TRANSLATION); //몬스터

			_float fToCover = XMVectorGetX(XMVector3Length(vCoverTranslation - vTranslation)); //학생와 엄폐물 사이의 거리

			if ((_float)m_pOwner->Get_StudentInfo().iRange > fToCover) //학생 인식범위 내에 엄폐물 있을 경우
			{
				_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vCoverTranslation)); //가까운 몬스터와 엄폐물 사이의 거리 

				if ((_float)m_pOwner->Get_StudentInfo().iRange > fLength) //가까운 몬스터와 엄폐물 사이의 거리가 인식 범위 내
				{

					if (m_fCoverMin > fToCover)
					{
						m_fCoverMin = fToCover;
						m_pTargetCover = pCover; //가장 가까운 엄폐물 탐색
					}

				}

			}

		}//for

		if (nullptr != m_pTargetCover) //엄폐물이 사용중이 아님
		{
			CTransform* pTargetTransform = (CTransform*)m_pTargetCover->Get_Component(TEXT("Com_Transform"));

			_vector		vTarget = pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);

			_vector		vPosition = pStudentTransform->Get_State(CTransform::STATE_TRANSLATION);

			_vector		vLook = vTarget - vPosition;

			pStudentTransform->LookAtLerp(vTarget, 0.5f, fTimeDelta);


			if (0.5f < XMVectorGetX(XMVector3Length(vLook)))
			{
				pStudentTransform->Chase(vTarget, fTimeDelta);
				return nullptr;
			}
			else
			{
				return CRun_ToHide::Create(m_pOwner, m_pTargetMonster, m_pTargetCover);
			}

		}
		else
			return nullptr; //엄폐물이 없음 run to knee
	}
	
	pStudentTransform->Go_Straight(fTimeDelta); //범위 내 몬스터 없을시
	_vector vXY = pStudentTransform->Get_WorldMatrix().r[3];
	pStudentTransform->LookAtLerp(XMVectorSet(XMVectorGetX(vXY), XMVectorGetY(vXY), 65.f, 1.f), 1.f, fTimeDelta);


	return nullptr;
}

CRun * CRun::Create(CStudent * pOwner)
{

	return new CRun(pOwner);
}