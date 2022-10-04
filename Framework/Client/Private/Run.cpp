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

		_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vTranslation)); //�л��� ��� ���� ������ �Ÿ� 

		if ((_float)m_pOwner->Get_StudentInfo().iRange > fLength) //���̾��� ���� �˻� �� ���� �� ���� 
		{
			if (m_fMin > fLength)
			{
				m_fMin = fLength;
				m_pTargetMonster = pMonster; //���� ����� ����
			}
			m_pOwner->In_RangeMonsters(pMonster);
		}
		else
			m_pTargetMonster = nullptr;
	}

	if (nullptr != m_pTargetMonster)
	{

		//��ֹ� �˻� > ���� �ȿ� ��ֹ��� ������ ��ֹ��� ���� ���������� �Ÿ��� irange �̳��� ��� ��ֹ��� �̵���

		_uint iCoverCount = pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Cover"));

		//if (0 >= iCoverCount)
		//	return nullptr; // run to knee

		for (_uint i = 0; i < iCoverCount; ++i)
		{
			CForkLift* pCover = (CForkLift*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cover"), i);

			CTransform* pCoverTransform = (CTransform*)pCover->Get_Component(TEXT("Com_Transform"));

			CTransform* pMonsterTransform = (CTransform*)m_pTargetMonster->Get_Component(TEXT("Com_Transform"));

			_vector vCoverTranslation = pCoverTransform->Get_State(CTransform::STATE_TRANSLATION); //����
			_vector vMonsterTranslation = pMonsterTransform->Get_State(CTransform::STATE_TRANSLATION); //����

			_float fToCover = XMVectorGetX(XMVector3Length(vCoverTranslation - vTranslation)); //�л��� ���� ������ �Ÿ�

			if ((_float)m_pOwner->Get_StudentInfo().iRange > fToCover) //�л� �νĹ��� ���� ���� ���� ���
			{
				_float fLength = XMVectorGetX(XMVector3Length(vMonsterTranslation - vCoverTranslation)); //����� ���Ϳ� ���� ������ �Ÿ� 

				if ((_float)m_pOwner->Get_StudentInfo().iRange > fLength) //����� ���Ϳ� ���� ������ �Ÿ��� �ν� ���� ��
				{

					if (m_fCoverMin > fToCover)
					{
						m_fCoverMin = fToCover;
						m_pTargetCover = pCover; //���� ����� ���� Ž��
					}

				}

			}

		}//for

		if (nullptr != m_pTargetCover) //������ ������� �ƴ�
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
			return nullptr; //������ ���� run to knee
	}
	
	pStudentTransform->Go_Straight(fTimeDelta); //���� �� ���� ������
	_vector vXY = pStudentTransform->Get_WorldMatrix().r[3];
	pStudentTransform->LookAtLerp(XMVectorSet(XMVectorGetX(vXY), XMVectorGetY(vXY), 65.f, 1.f), 1.f, fTimeDelta);


	return nullptr;
}

CRun * CRun::Create(CStudent * pOwner)
{

	return new CRun(pOwner);
}