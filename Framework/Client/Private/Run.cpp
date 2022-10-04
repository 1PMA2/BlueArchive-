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
#include "Run_ToKnee.h"

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
	m_bOnce = true;
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
		
		Find_Cover();

		for (_int i = 0; i < 4; ++i)
		{
			if (nullptr != m_pTargetCover[i] && !m_pTargetCover[i]->Get_Use()) //엄폐물이 사용중이 아님
			{

				CTransform* pTargetTransform = (CTransform*)m_pTargetCover[i]->Get_Component(TEXT("Com_Transform"));

				_vector		vTarget = pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);

				_vector		vPosition = pStudentTransform->Get_State(CTransform::STATE_TRANSLATION);

				_vector		vLook = vTarget - vPosition;

				pStudentTransform->LookAtLerp(vTarget, 3.f, fTimeDelta);


				if (0.5f < XMVectorGetX(XMVector3Length(vLook)))
				{
					pStudentTransform->Go_Straight(fTimeDelta); //Chase(vTarget, fTimeDelta);
					return nullptr;
				}
				else
				{
					m_pTargetCover[i]->Set_Use(true);
					return CRun_ToHide::Create(m_pOwner, m_pTargetMonster, m_pTargetCover[i]);
				}

			}
			else if (nullptr == m_pTargetCover)
			{
				return CRun_ToKnee::Create(m_pOwner); //엄폐물이 없음 run to knee
			}


		}
		
	}
	

	pStudentTransform->Go_Straight(fTimeDelta); //범위 내 몬스터 없을시
	_vector vXY = pStudentTransform->Get_WorldMatrix().r[3];
	pStudentTransform->LookAtLerp(XMVectorSet(XMVectorGetX(vXY), XMVectorGetY(vXY), 65.f, 1.f), 1.f, fTimeDelta);


	return nullptr;
}

void CRun::Find_Cover()
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();

	CTransform* pStudentTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	_vector vTranslation = pStudentTransform->Get_State(CTransform::STATE_TRANSLATION);

	_uint iCoverCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Cover"));

	if (0 >= iCoverCount)
		return;

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

			if ((_float)m_pOwner->Get_StudentInfo().iRange > (fLength)) //가까운 몬스터와 엄폐물 사이의 거리가 인식 범위 내
			{

				m_pTargetCover[i] = pCover; //가장 가까운 엄폐물 탐색,사용중이 아닌

			}

		}

	}
}

CRun * CRun::Create(CStudent * pOwner)
{

	return new CRun(pOwner);
}