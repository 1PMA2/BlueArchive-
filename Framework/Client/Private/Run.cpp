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
#include "Collider.h"

CRun::CRun(CStudent* pOwner)
	:CState(pOwner)
{
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));
	m_eAnim = ANIM_RUN;
		pOwner->Set_State(m_eAnim);

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case 0:
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		m_fHideLength = 0.35f;
		break;
	case 1:
		pModel->Set_CurrentAnimation(7);
		m_fHideLength = 0.1f;
		break;
	case 2:
		pModel->Set_CurrentAnimation(7);
		m_fHideLength = 0.1f;
		break;

	}
	
}


CRun::~CRun()
{
}

void CRun::Enter()
{
	m_bOnce = true;
	m_bCheck = true;
	m_iIndex = 0;
}

CState * CRun::Loop(_float fTimeDelta)
{
	CState* pState = __super::Loop(fTimeDelta);

	if (nullptr != pState)
		return pState;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CTransform* pTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	CModel* pModel = (CModel*)m_pOwner->Get_Component(TEXT("Com_Model"));

	pModel->Repeat_Animation(fTimeDelta);

	//CollisionCover(fTimeDelta); //보류
	pState = Find_Monster(fTimeDelta);


	return pState;
}

void CRun::Exit()
{
	m_TargetCovers.clear();
	m_TargetMonsters.clear();
	Destroy_Instance();
}

CState* CRun::Find_Monster(_float fTimeDelta)
{
	m_TargetMonsters.clear();

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();

	CTransform* pStudentTransform = (CTransform*)m_pOwner->Get_Component(TEXT("Com_Transform"));

	_uint iMonsterCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

	_vector vTranslation = pStudentTransform->Get_State(CTransform::STATE_TRANSLATION);

	if (0 >= iMonsterCount)
	{
		pStudentTransform->Go_Straight(fTimeDelta);
		_vector vXY = pStudentTransform->Get_WorldMatrix().r[3];
		pStudentTransform->LookAtLerp(XMVectorSet(XMVectorGetX(vXY), XMVectorGetY(vXY), 65.f, 1.f), 5.f, fTimeDelta);
		return nullptr;
	}
	

	for (_uint i = 0; i < iMonsterCount; ++i)
	{
		CMonster* pMonster = (CMonster*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), i);

		CTransform* pMonsterTransform = (CTransform*)pMonster->Get_Component(TEXT("Com_Transform"));

		_vector vMonsterTranslation = pMonsterTransform->Get_State(CTransform::STATE_TRANSLATION);

		_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vTranslation)); //학생과 모든 몬스터 사이의 거리 

		if (m_pOwner->Get_StudentInfo().fReConRange > fLength) //레이어의 몬스터 검사 후 인식범위 내 몬스터 
		{
			m_TargetMonsters.push_back(pMonster); //범위 내 몬스터
		}
	}


	if (0 < m_TargetMonsters.size())
	{

		Find_Cover();

		if (0 < m_TargetCovers.size()) //엄폐물 탐색 성공시
		{
			if (m_bOnce)
				{
					_uint iRandom = random(0, m_TargetCovers.size() - 1);
					m_pTargetCover = m_TargetCovers.at(iRandom);
					m_pTargetCover->Set_Use();
					m_bOnce = false;
				}
		}

		if(nullptr != m_pTargetCover) //값이 제대로 들어갔다면
		{

			CTransform* pTargetTransform = (CTransform*)m_pTargetCover->Get_Component(TEXT("Com_Transform"));

			_vector		vTarget = pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);

			_vector		vPosition = pStudentTransform->Get_State(CTransform::STATE_TRANSLATION);

			_vector		vLook = vTarget - vPosition;

			if (-1.f > XMVectorGetZ(vLook))
			{
				m_pTargetCover = nullptr;
				m_bOnce = true;
				return nullptr;
			}

			pStudentTransform->LookAtLerp(vTarget, 5.f, fTimeDelta); // 엄폐물 방향 look


			if (m_fHideLength <= XMVectorGetX(XMVector3Length(vLook)))
			{
				pStudentTransform->Go_Straight(fTimeDelta); //Chase(vTarget, fTimeDelta); 엄폐물에 닿을때 까지 앞으로
				return nullptr;
			}
			else
			{
				return CRun_ToHide::Create(m_pOwner, m_TargetMonsters.at(0), m_pTargetCover);
			}

		}
		else //엄폐물이 없음
		{
			_vector		vTarget = m_TargetMonsters.at(0)->Get_MonsterTranslation(); 

			pStudentTransform->LookAtLerp(vTarget, 5.f, fTimeDelta);

			if(false == m_pOwner->Get_IsColl())
			{
				if (m_pOwner->Get_StudentInfo().fRange > XMVectorGetX(XMVector3Length(vTarget - vTranslation))) //몬스터 방향으로 공격범위까지 이동완료
					return CRun_ToKnee::Create(m_pOwner);
				else
				{
					pStudentTransform->Go_Straight(fTimeDelta); //몬스터 방향으로 공격범위까지 이동
					return nullptr;
				}
			}
		}

	}
	

	pStudentTransform->Go_Straight(fTimeDelta); //범위 내 몬스터 없을시 앞으로 전진
	_vector vXY = pStudentTransform->Get_WorldMatrix().r[3];
	pStudentTransform->LookAtLerp(XMVectorSet(XMVectorGetX(vXY), XMVectorGetY(vXY), 65.f, 1.f), 5.f, fTimeDelta);


	return nullptr;
}

void CRun::Find_Cover()
{
	if (nullptr != m_pTargetCover)
	{
		return;
	}

	m_TargetCovers.clear();

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

		CTransform* pMonsterTransform = (CTransform*)m_TargetMonsters.at(0)->Get_Component(TEXT("Com_Transform"));

		_vector vCoverTranslation = pCoverTransform->Get_State(CTransform::STATE_TRANSLATION); //엄폐물
		_vector vMonsterTranslation = pMonsterTransform->Get_State(CTransform::STATE_TRANSLATION); //몬스터

		_float fToCover = XMVectorGetX(XMVector3Length(vCoverTranslation - vTranslation)); //학생와 엄폐물 사이의 거리

		if (m_pOwner->Get_StudentInfo().fRange > fToCover) //학생 공격범위 내에 엄폐물 있을 경우
		{
			_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vCoverTranslation)); //가까운 몬스터와 엄폐물 사이의 거리 

			if (m_pOwner->Get_StudentInfo().fRange > (fLength)) //가까운 몬스터와 엄폐물 사이의 거리가 공격 범위 내
			{
				if (false == pCover->Get_Use())
					m_TargetCovers.push_back(pCover); //가장 가까운 엄폐물 탐색,사용중이 아닌
			}

		}

	}
}

CRun * CRun::Create(CStudent * pOwner)
{

	return new CRun(pOwner);
}