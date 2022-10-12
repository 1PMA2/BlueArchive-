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

	//CollisionCover(fTimeDelta); //����
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

		_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vTranslation)); //�л��� ��� ���� ������ �Ÿ� 

		if (m_pOwner->Get_StudentInfo().fReConRange > fLength) //���̾��� ���� �˻� �� �νĹ��� �� ���� 
		{
			m_TargetMonsters.push_back(pMonster); //���� �� ����
		}
	}


	if (0 < m_TargetMonsters.size())
	{

		Find_Cover();

		if (0 < m_TargetCovers.size()) //���� Ž�� ������
		{
			if (m_bOnce)
				{
					_uint iRandom = random(0, m_TargetCovers.size() - 1);
					m_pTargetCover = m_TargetCovers.at(iRandom);
					m_pTargetCover->Set_Use();
					m_bOnce = false;
				}
		}

		if(nullptr != m_pTargetCover) //���� ����� ���ٸ�
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

			pStudentTransform->LookAtLerp(vTarget, 5.f, fTimeDelta); // ���� ���� look


			if (m_fHideLength <= XMVectorGetX(XMVector3Length(vLook)))
			{
				pStudentTransform->Go_Straight(fTimeDelta); //Chase(vTarget, fTimeDelta); ���󹰿� ������ ���� ������
				return nullptr;
			}
			else
			{
				return CRun_ToHide::Create(m_pOwner, m_TargetMonsters.at(0), m_pTargetCover);
			}

		}
		else //������ ����
		{
			_vector		vTarget = m_TargetMonsters.at(0)->Get_MonsterTranslation(); 

			pStudentTransform->LookAtLerp(vTarget, 5.f, fTimeDelta);

			if(false == m_pOwner->Get_IsColl())
			{
				if (m_pOwner->Get_StudentInfo().fRange > XMVectorGetX(XMVector3Length(vTarget - vTranslation))) //���� �������� ���ݹ������� �̵��Ϸ�
					return CRun_ToKnee::Create(m_pOwner);
				else
				{
					pStudentTransform->Go_Straight(fTimeDelta); //���� �������� ���ݹ������� �̵�
					return nullptr;
				}
			}
		}

	}
	

	pStudentTransform->Go_Straight(fTimeDelta); //���� �� ���� ������ ������ ����
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

		_vector vCoverTranslation = pCoverTransform->Get_State(CTransform::STATE_TRANSLATION); //����
		_vector vMonsterTranslation = pMonsterTransform->Get_State(CTransform::STATE_TRANSLATION); //����

		_float fToCover = XMVectorGetX(XMVector3Length(vCoverTranslation - vTranslation)); //�л��� ���� ������ �Ÿ�

		if (m_pOwner->Get_StudentInfo().fRange > fToCover) //�л� ���ݹ��� ���� ���� ���� ���
		{
			_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vCoverTranslation)); //����� ���Ϳ� ���� ������ �Ÿ� 

			if (m_pOwner->Get_StudentInfo().fRange > (fLength)) //����� ���Ϳ� ���� ������ �Ÿ��� ���� ���� ��
			{
				if (false == pCover->Get_Use())
					m_TargetCovers.push_back(pCover); //���� ����� ���� Ž��,������� �ƴ�
			}

		}

	}
}

CRun * CRun::Create(CStudent * pOwner)
{

	return new CRun(pOwner);
}