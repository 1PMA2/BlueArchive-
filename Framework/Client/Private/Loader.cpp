#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "LoadingImage.h"
#include "BG_Lobby.h"
#include "Camera_Free.h"
#include "Camera_Ex.h"
#include "Monster.h"
#include "Formation_Terrain.h"
#include "Terrain.h"
#include "Aru.h"
#include "Aru_Ex.h"
#include "Mutsuki.h"
//#include "Effect.h"
#include "Sky.h"
#include "Sensei.h"
#include "ForkLift.h"
#include "City.h"
#include "UI.h"
#include "UI_RecruitButton.h"
#include "Camera_Gacha.h"
#include "Arona.h"
#include "Arona_CAM.h"
#include "Arona_Sack.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	HRESULT hr = 0;

		switch (pLoader->Get_NextLevelID())
		{
		case LEVEL_LOGO:
			hr = pLoader->Loading_ForLogoLevel();
			break;
		case LEVEL_LOBBY:
			hr = pLoader->Loading_ForLobbyLevel();
			break;
		case LEVEL_FORMATION:
			hr = pLoader->Loading_ForFormationLevel();
			break;
		case LEVEL_GACHA:
			hr = pLoader->Loading_ForGachaLevel();
			break;
		case LEVEL_GACHASCENE:
			hr = pLoader->Loading_ForGachaScene();
			break;
		case LEVEL_GAMEPLAY:
			hr = pLoader->Loading_ForGamePlayLevel();
			break;
		}


	if (FAILED(hr))
		MSG_BOX("Failed to Loading");

	LeaveCriticalSection(&pLoader->Get_CS());

	CSensei* pSensei = GET_INSTANCE(CSensei);

	pSensei->Set_CurrentLevel(pLoader->Get_NextLevelID());

	RELEASE_INSTANCE(CSensei);

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	
#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingImage"),
		CLoadingImage::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중이빈다. "));
	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCubeTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements))))
		return E_FAIL;

	

#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Fade.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Loading%d.jpg"), 2))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));


	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));	

	m_isFinished = true;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForLobbyLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

#pragma region PROTOTYPE_GAMEOBJECT
	if (g_bLobby) //객체 개수, 재생성 막기
	{

		lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

		/* For.Prototype_GameObject_Lobby */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BG_Lobby"),
			CBG_Lobby::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GachaButton"),
			CUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Student */
		g_bLobby = false;
	}
#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_BG_Lobby"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Bg_Lobby%d.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_GachaButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Gacha.png"), 1))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_WorkButton"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Gacha.png"), 1))))
	//	return E_FAIL;
	
	

	lstrcpy(m_szLoadingText, TEXT("lobby로딩 끝 "));

	m_isFinished = true;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForGachaLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (g_bGacha) //객체 개수, 재생성 막기
	{

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RecruitButton"),
			CUI_RecruitButton::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Gacha"),
			CCamera_Gacha::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Arona"),
			CArona::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AronaCAM"),
			CArona_CAM::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AronaSack"),
			CArona_Sack::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Camera_Free*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
			CCamera_Free::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		_matrix TransformMatrix;

		ZeroMemory(&TransformMatrix, sizeof(_matrix));
		TransformMatrix = XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Arona"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Arona/", "Arona.fbx", TransformMatrix))))
			return E_FAIL;

		ZeroMemory(&TransformMatrix, sizeof(_matrix));
		TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_AronaCAM"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Arona/", "CAM.fbx", TransformMatrix))))
			return E_FAIL;

		ZeroMemory(&TransformMatrix, sizeof(_matrix));
		TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_AronaSack"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Arona/", "Sack.fbx", TransformMatrix))))
			return E_FAIL;

		g_bGacha = false;
	}

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA, TEXT("Prototype_Component_Texture_Recruit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/recruit.png"), 1))))
		return E_FAIL;

	





	lstrcpy(m_szLoadingText, TEXT("gacha 로딩 끝 "));

	m_isFinished = true;
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForGachaScene()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	//
	lstrcpy(m_szLoadingText, TEXT("model"));
	_matrix			TransformMatrix;

	CSensei* pSensei = GET_SENSEI;

	for (_int i = 0; i < pSensei->Get_StudentNum(); ++i)
	{
		if (nullptr == pSensei->Get_StudentIndex(i))
		{
			if (TEXT("Aru") == pSensei->Get_StudentName(i))
			{
				pSensei->Set_RealStudent(pSensei->Get_StudentName(i), CAru::Create(m_pDevice, m_pContext));
				if (FAILED(pGameInstance->Add_Prototype(pSensei->Get_StudentName(i),
					pSensei->Get_StudentIndex(i))))
					return E_FAIL;

				ZeroMemory(&TransformMatrix, sizeof(_matrix));
				TransformMatrix = XMMatrixScaling(100.f, 100.f, 100.f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Aru"),
					CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Aru/", "Aru.fbx", TransformMatrix))))
					return E_FAIL;
			}

			if (TEXT("Mutsuki") == pSensei->Get_StudentName(i))
			{
				pSensei->Set_RealStudent(pSensei->Get_StudentName(i), CMutsuki::Create(m_pDevice, m_pContext));
				if (FAILED(pGameInstance->Add_Prototype(pSensei->Get_StudentName(i),
					pSensei->Get_StudentIndex(i))))
					return E_FAIL;

				ZeroMemory(&TransformMatrix, sizeof(_matrix));
				TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Mutsuki"),
					CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Mutsuki/", "Mutsuki.fbx", TransformMatrix))))
					return E_FAIL;
			}
		}

	}
	
	//
	m_isFinished = true;
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForFormationLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (g_bFormation) //객체 개수, 재생성 막기
	{
#pragma region PROTOTYPE_GAMEOBJECT

		lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));


		/* For.Prototype_GameObject_Terrain*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Formation_Terrain"),
			CFormation_Terrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Student */




		lstrcpy(m_szLoadingText, TEXT("콜라이더추가.  "));
		/* For.Prototype_Component_Collider_AABB */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
			CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_OBB */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
			CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_SPHERE */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
			CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Terrain */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2))))
			return E_FAIL;

		lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));


		/* For.Prototype_Component_VIBuffer_Terrain */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 10, 10))))
			return E_FAIL;
		


		g_bFormation = false;
	}
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("편성 로딩 끝 "));

	m_isFinished = true;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();

	CSensei* pSensei = GET_SENSEI;
	Safe_AddRef(pGameInstance);


	if (g_bPlay) //객체 개수, 재생성 막기
	{

#pragma region PROTOTYPE_GAMEOBJECT

		lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
			CTerrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_City"),
			CCity::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Sky */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
			CSky::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		///* For.Prototype_GameObject_Student */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Aru"),
		//	CAru::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Aru_Ex"),
			CAru_Ex::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Camera_Ex*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Ex"),
			CCamera_Ex::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ForkLift"),
			CForkLift::Create(m_pDevice, m_pContext))))
			return E_FAIL;


		///* For.Prototype_GameObject_Monster */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
			CMonster::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		///* For.Prototype_GameObject_Effect */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
		//	CEffect::Create(m_pGraphic_Device))))
		//	return E_FAIL;


#pragma endregion

		g_bPlay = false;
	}

		lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));



		/* For.Prototype_Component_Texture_Sky */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
			return E_FAIL;


		///* For.Prototype_Component_Texture_Filter */
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Filter"),
		//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.bmp"), 1))))
		//	return E_FAIL;

		///* For.Prototype_Component_Texture_Brush */
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
		//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		//	return E_FAIL;

		lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

		/* For.Prototype_Component_VIBuffer_Cube*/
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
			CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
			return E_FAIL;


		_matrix			TransformMatrix;

		/* For.Prototype_Component_Model_ForkLift*/
		TransformMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(90.0f));

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ForkLift/", "ForkLift.fbx", TransformMatrix))))
			return E_FAIL;
		
		ZeroMemory(&TransformMatrix, sizeof(_matrix));
		TransformMatrix = XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Aru_Ex"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Aru/", "Aru_Ex.fbx", TransformMatrix))))
			return E_FAIL;

		ZeroMemory(&TransformMatrix, sizeof(_matrix));
		TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_City"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/map/", "City.fbx", TransformMatrix))))
			return E_FAIL;


		

		lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));


	m_isFinished = true;

	
	Safe_Release(pGameInstance);
	return S_OK;
}


CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
}
