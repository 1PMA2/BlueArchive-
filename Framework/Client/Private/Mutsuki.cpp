#include "stdafx.h"
#include "..\Public\Mutsuki.h"

#include "GameInstance.h"
#include "Run_Sign.h"
#include "Run.h"
#include "Sensei.h"
#include "Formation_Idle.h"

CMutsuki::CMutsuki(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CStudent(pDevice, pContext)
{
}

CMutsuki::CMutsuki(const CMutsuki & rhs)
	: CStudent(rhs)
{
}

HRESULT CMutsuki::Initialize_Prototype()
{
	m_tStudentInfo.pName = TEXT("Mutsuki");
	m_tStudentInfo.bExModel = false;
	m_tStudentInfo.fFireSpeed = 0.2f;
	m_tStudentInfo.eStudent = MUTSUKI;
	return S_OK;
}

HRESULT CMutsuki::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(180.0f);
	
	m_tStudentInfo.eAnim = ANIM_KNEEZOOMFIRE;
	m_tStudentInfo.eFormation = FORMATION_SECOND;
	m_tStudentInfo.iAtk = 15;
	m_tStudentInfo.iDef = 0;
	m_tStudentInfo.iEx = 30;
	m_tStudentInfo.fExCost = 4.f;
	m_tStudentInfo.iHp = 100;
	m_tStudentInfo.iMagazine = 10;
	m_tStudentInfo.iBullet = 10;
	m_tStudentInfo.fRange = 5.f;
	m_tStudentInfo.fReConRange = 10.f;
	m_tStudentInfo.iShield = 0;

	m_bExReady = false;
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;	

	return S_OK;
}

void CMutsuki::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);

}

void CMutsuki::LateTick(_float fTimeDelta)
{

	
	CSensei* pSensei = CSensei::Get_Instance();

	switch (pSensei->Get_CurrentLevel())
	{
	case LEVEL_FORMATION:
		FormationLevel_Collision();
		break;
	case LEVEL_GAMEPLAY:
		GamePlayLevel_Collision();
		break;
	}

	

	__super::LateTick(fTimeDelta);
}

HRESULT CMutsuki::Render()
{
	__super::Render();

	//CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	//swprintf_s(m_szFPS, TEXT("cost:%d"), m_tStudentInfo.iBullet);

	//// MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont
	//pGameInstance->Render_Font(TEXT("Font_Dream"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));


	//pGameInstance->Present();

	return S_OK;
}

HRESULT CMutsuki::SetUp_Components()
{
	__super::SetUp_Components();

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(0.5f, 1.f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Mutsuki"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMutsuki::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}

HRESULT CMutsuki::FormationLevel_Collision()
{


	__super::FormationLevel_Collision();

	return S_OK;
}

HRESULT CMutsuki::GamePlayLevel_Collision()
{
	__super::GamePlayLevel_Collision();
	

	if (m_pAABBCom->CollisionRay())
	{
		CSensei* pSensei = GET_SENSEI;
		if (KEY(LBUTTON, TAP))
		{
			pSensei->Set_ExReady();
		}
	}

	return S_OK;
}

void CMutsuki::OnDisable()
{
}

void CMutsuki::OnEnable()
{
}

CMutsuki * CMutsuki::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMutsuki*		pInstance = new CMutsuki(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMutsuki");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CMutsuki::Clone(void * pArg)
{
	CMutsuki*		pInstance = new CMutsuki(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMutsuki");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMutsuki::Free()
{
	__super::Free();
}
