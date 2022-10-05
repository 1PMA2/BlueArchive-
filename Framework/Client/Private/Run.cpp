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
	CModel* pModel = (CModel*)pOwner->Get_Component(TEXT("Com_Model"));

	switch (pOwner->Get_StudentInfo().eStudent)
	{
	case 0:
		m_eAnim = ANIM_RUN;
		pOwner->Set_State(m_eAnim);
		pModel->Set_CurrentAnimation(pOwner->Get_StudentInfo().eAnim);
		m_fHideLength = 0.5f;
		break;
	case 1:
		pModel->Set_CurrentAnimation(7);
		m_fHideLength = 0.1f;
		break;
	case 2:
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
	m_TargetMonsters.clear();

	for (_uint i = 0; i < iMonsterCount; ++i)
	{
		CMonster* pMonster = (CMonster*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), i);

		CTransform* pMonsterTransform = (CTransform*)pMonster->Get_Component(TEXT("Com_Transform"));

		_vector vMonsterTranslation = pMonsterTransform->Get_State(CTransform::STATE_TRANSLATION);

		_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vTranslation)); //학생과 모든 몬스터 사이의 거리 

		if ((_float)m_pOwner->Get_StudentInfo().iRange > fLength) //레이어의 몬스터 검사 후 범위 내 몬스터 
		{
			m_TargetMonsters.push_back(pMonster); //범위 내 몬스터
		}
	}


	if (0 < m_TargetMonsters.size())
	{

		Find_Cover();

		CStudent* pStudent;

		if(m_bOnce)
		{ 
			for (int i = 0; i < 2; ++i) // formationstudent.size();
			{
				pStudent = (CStudent*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Student"), i); //최대 4명 등록이기때문에 ㄱㅊ

				if (m_pOwner != pStudent)
				{
					if (pStudent->FoundObstacle())
					{
						++m_iIndex;
						break;
					}
				}
			}
			m_bOnce = false;
		}
		

		if(0 < m_TargetCovers.size() && m_iIndex + 1 <= m_TargetCovers.size())
		{
			m_pOwner->Set_Cover(true);

			CTransform* pTargetTransform = (CTransform*)m_TargetCovers.at(m_iIndex)->Get_Component(TEXT("Com_Transform"));

			_vector		vTarget = pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);

			_vector		vPosition = pStudentTransform->Get_State(CTransform::STATE_TRANSLATION);

			_vector		vLook = vTarget - vPosition;

			pStudentTransform->LookAtLerp(vTarget, 5.f, fTimeDelta);


			if (m_fHideLength <= XMVectorGetX(XMVector3Length(vLook)))
			{
				pStudentTransform->Go_Straight(fTimeDelta); //Chase(vTarget, fTimeDelta);
				return nullptr;
			}
			else
			{
				return CRun_ToHide::Create(m_pOwner, m_TargetMonsters.at(0), m_TargetCovers.at(m_iIndex));
			}

		}
		else
		{
			return CRun_ToKnee::Create(m_pOwner);
		}

	}
	

	pStudentTransform->Go_Straight(fTimeDelta); //범위 내 몬스터 없을시
	_vector vXY = pStudentTransform->Get_WorldMatrix().r[3];
	pStudentTransform->LookAtLerp(XMVectorSet(XMVectorGetX(vXY), XMVectorGetY(vXY), 65.f, 1.f), 1.f, fTimeDelta);


	return nullptr;
}

void CRun::Find_Cover()
{
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

		if ((_float)m_pOwner->Get_StudentInfo().iRange > fToCover) //학생 인식범위 내에 엄폐물 있을 경우
		{
			_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vCoverTranslation)); //가까운 몬스터와 엄폐물 사이의 거리 

			if ((_float)m_pOwner->Get_StudentInfo().iRange > (fLength)) //가까운 몬스터와 엄폐물 사이의 거리가 인식 범위 내
			{

				m_TargetCovers.push_back(pCover); //가장 가까운 엄폐물 탐색,사용중이 아닌

			}

		}

	}
}

CRun * CRun::Create(CStudent * pOwner)
{

	return new CRun(pOwner);
}