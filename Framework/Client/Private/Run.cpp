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
	
	CSensei* pSensei = CSensei::Get_Instance();


	if (pModel->Get_isFinished())
	{
		return CRun::Create(m_pOwner);
	}

	
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
	}

	if (0 < m_pOwner->Get_InRangeMonsters())
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

			_float4 f;

			XMStoreFloat4(&f, pTargetTransform->Get_WorldMatrix().r[3]);

			_vector		vTarget;

			vTarget = XMVectorSet(f.x, f.y, f.z - 0.7f, f.w); // �ݶ��̴� ��ġ �� ���� �ʿ�

			_vector		vPosition = pStudentTransform->Get_State(CTransform::STATE_TRANSLATION);

			_vector		vLook = vTarget - vPosition;

			_float		fAngle;

			fAngle = (acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(vLook),
				XMVector3Normalize(pStudentTransform->Get_State(CTransform::STATE_LOOK))))));

			if (0 > XMVectorGetX(vLook))
				pStudentTransform->TurnFor(XMVectorSet(0.f, -1.f, 0.f, 0.f), fTimeDelta, fAngle);
			else
				pStudentTransform->TurnFor(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta, fAngle);




			if (1.f < XMVectorGetX(XMVector3Length(vLook)))
			{
				pStudentTransform->Chase(vTarget, fTimeDelta);
				return nullptr;
			}
			else
			{
				return CRun_ToHide::Create(m_pOwner, m_pTargetMonster);
			}

		}
		else
			return nullptr; //������ ���� run to knee
	}

	pStudentTransform->Go_Straight(fTimeDelta); //���� �� ���� ������



	return nullptr;
}

CRun * CRun::Create(CStudent * pOwner)
{
	return new CRun(pOwner);
}