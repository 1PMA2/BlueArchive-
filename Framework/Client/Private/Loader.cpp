#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Fade_Out.h"
#include "Fade_InOut.h"
#include "LoadingImage.h"
#include "BG_Lobby.h"
#include "BG_Gacha.h"
#include "BG_Pickup.h"
#include "Camera_Free.h"
#include "Camera_Main.h"
#include "Camera_Ex.h"
#include "Monster_Droid.h"
#include "Monster_Boss.h"
#include "Formation_Terrain.h"
#include "Terrain.h"
#include "Aru.h"
#include "Aru_Ex.h"
#include "Aru_ExBullet.h"
#include "Mutsuki.h"
#include "Mutsuki_Ex.h"
#include "Mutsuki_ExBullet.h"
#include "Kayoko.h"
#include "Kayoko_Ex.h"
#include "Haruka.h"
#include "Haruka_Ex.h"
#include "Haruka_ExBullet.h"
//#include "Effect.h"
#include "Sky.h"
#include "Sky_Ex.h"
#include "Sensei.h"
#include "ForkLift.h"
#include "City.h"
#include "UI.h"
#include "UI_RecruitButton.h"
#include "Work_Button.h"
#include "Camera_Gacha.h"
#include "Arona.h"
#include "Arona_CAM.h"
#include "Arona_Sack.h"
#include "Student_Img.h"
#include "Back_Btn.h"
#include "Monster_Trigger.h"
#include "Boss_Trigger.h"
#include "Droid_Dead.h"
#include "Boss_Dead.h"
#include "Cost_Gauge.h"
#include "Cost_GaugeBg.h"
#include "Ex_Portrait.h"
#include "Camera_Aru.h"
#include "Camera_Mutsuki.h"
#include "Camera_Kayoko.h"
#include "Camera_Haruka.h"
#include "Warning.h"
#include "Warning_Bar.h"
#include "Boss_Bullet.h"
#include "Boss_SBullet.h"
#include "Victory_Bg.h"
#include "Victory.h"
#include "Formaton_Btn.h"
#include "Formation_Window.h"
#include "Student_Portrait.h"
#include "Muzzle.h"
#include "Monster_Muzzle.h"
#include "Bullet.h"
#include "Monster_Bullet.h"
#include "Smoke.h"
#include "SmokeL.h"
#include "Flare.h"
#include "Cylinder.h"
#include "Workspace.h"
#include "Slash.h"
#include "Damage.h"
#include "Boss_HP.h"

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

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FadeOut"),
		CFade_Out::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FadeInOut"),
		CFade_InOut::Create(m_pDevice, m_pContext))))
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

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Effect.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;


	/*Test */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Test"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Test.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	

#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Fade%d.png"), 2))))
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
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BG"),
			CBG_Lobby::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Workspace"),
			CWorkspace::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GachaButton"),
			CUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WorkButton"),
			CWork_Button::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackButton"),
			CBack_Btn::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Camera_Free*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
			CCamera_Free::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BG"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/BG%d.png"), 3))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BackButton"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Back_Btn.png"), 1))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Workpage"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/zzz/images/%d.png"), 150))))
			return E_FAIL;

		/* For.Prototype_GameObject_Student */
		g_bLobby = false;
	}
#pragma endregion

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_WorkButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Work.png"), 1))))
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

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BG_Pickup"),
			CBG_Pickup::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StudentImg"),
			CStudent_Img::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Slash"),
			CSlash::Create(m_pDevice, m_pContext))))
			return E_FAIL;

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

		lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
		/* For.Prototype_Component_Texture_Default */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Student"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Student/spr%d.png"), 4))))
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

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Video"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Video/gif/%d.png"), 200))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Slash"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/slash.png"), 1))))
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
	lstrcpy(m_szLoadingText, TEXT("New Student!!"));
	_matrix			TransformMatrix;

	CSensei* pSensei = GET_SENSEI;

	for (_uint i = 0; i < pSensei->Get_StudentNum(); ++i)
	{
		if (nullptr == pSensei->Get_StudentIndex(i))
		{
			if (TEXT("Aru") == pSensei->Get_StudentName(i))
			{
				pSensei->Set_RealStudent(pSensei->Get_StudentName(i), CAru::Create(m_pDevice, m_pContext));
				if (FAILED(pGameInstance->Add_Prototype(pSensei->Get_StudentName(i),
					pSensei->Get_StudentIndex(i))))
					return E_FAIL;

				if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Aru"),
					CCamera_Aru::Create(m_pDevice, m_pContext))))
					return E_FAIL;

				if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Aru_ExBullet"),
					CAru_ExBullet::Create(m_pDevice, m_pContext))))
					return E_FAIL;

				ZeroMemory(&TransformMatrix, sizeof(_matrix));
				TransformMatrix = XMMatrixScaling(100.f, 100.f, 100.f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Aru"),
					CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Aru/", "Aru.fbx", TransformMatrix))))
					return E_FAIL;


			}

			else if (TEXT("Mutsuki") == pSensei->Get_StudentName(i))
			{
				pSensei->Set_RealStudent(pSensei->Get_StudentName(i), CMutsuki::Create(m_pDevice, m_pContext));
				if (FAILED(pGameInstance->Add_Prototype(pSensei->Get_StudentName(i),
					pSensei->Get_StudentIndex(i))))
					return E_FAIL;

				if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Mutsuki"),
					CCamera_Mutsuki::Create(m_pDevice, m_pContext))))
					return E_FAIL;

				if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mutsuki_ExBullet"),
					CMutsuki_ExBullet::Create(m_pDevice, m_pContext))))
					return E_FAIL;

				ZeroMemory(&TransformMatrix, sizeof(_matrix));
				TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Mutsuki"),
					CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Mutsuki/", "Mutsuki.fbx", TransformMatrix))))
					return E_FAIL;
			}

			else if (TEXT("Kayoko") == pSensei->Get_StudentName(i))
			{
				pSensei->Set_RealStudent(pSensei->Get_StudentName(i), CKayoko::Create(m_pDevice, m_pContext));
				if (FAILED(pGameInstance->Add_Prototype(pSensei->Get_StudentName(i),
					pSensei->Get_StudentIndex(i))))
					return E_FAIL;

				if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Kayoko"),
					CCamera_Kayoko::Create(m_pDevice, m_pContext))))
					return E_FAIL;

				ZeroMemory(&TransformMatrix, sizeof(_matrix));
				TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Kayoko"),
					CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Kayoko/", "Kayoko.fbx", TransformMatrix))))
					return E_FAIL;
			}

			else if (TEXT("Haruka") == pSensei->Get_StudentName(i))
			{
				pSensei->Set_RealStudent(pSensei->Get_StudentName(i), CHaruka::Create(m_pDevice, m_pContext));
				if (FAILED(pGameInstance->Add_Prototype(pSensei->Get_StudentName(i),
					pSensei->Get_StudentIndex(i))))
					return E_FAIL;

				if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Haruka"),
					CCamera_Haruka::Create(m_pDevice, m_pContext))))
					return E_FAIL;

				if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Haruka_ExBullet"),
					CHaruka_ExBullet::Create(m_pDevice, m_pContext))))
					return E_FAIL;

				ZeroMemory(&TransformMatrix, sizeof(_matrix));
				TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Haruka"),
					CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Haruka/", "Haruka.fbx", TransformMatrix))))
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

		/* For.Prototype_GameObject_Sky */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
			CSky::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Terrain*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Formation_Terrain"),
			CFormation_Terrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Formation_Btn"),
			CFormaton_Btn::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Formation_Window"),
			CFormation_Window::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portrait"),
			CStudent_Portrait::Create(m_pDevice, m_pContext))))
			return E_FAIL;


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
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 50, 100))))
			return E_FAIL;

		/* For.Prototype_Component_VIBuffer_Cube*/
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
			CVIBuffer_Cube::Create(m_pDevice, m_pContext, 15.f))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Instance"),
			CVIBuffer_Rect_Instance::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		
		lstrcpy(m_szLoadingText, TEXT("sky tex. "));
		/* For.Prototype_Component_Texture_Sky */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 5))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portrait"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Portrait%d.png"), 4))))
			return E_FAIL;


		g_bFormation = false;
	}

#pragma endregion
	lstrcpy(m_szLoadingText, TEXT("formation tex. "));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FORMATION, TEXT("Prototype_Component_Texture_FormationBtn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/FormationBtn.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FORMATION, TEXT("Prototype_Component_Texture_FormationWindow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/FormationSet.png"), 1))))
		return E_FAIL;

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

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Damage"),
			CDamage::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky_Ex"),
			CSky_Ex::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CostGaugeBg"),
			CCost_GaugeBg::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CostGauge"),
			CCost_Gauge::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ex_Portrait"),
			Ex_Portrait::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Trigger"),
			CMonster_Trigger::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BossTrigger"),
			CBoss_Trigger::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Bullet"),
			CBoss_Bullet::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_SBullet"),
			CBoss_SBullet::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Dead"),
			CDroid_Dead::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BossDead"),
			CBoss_Dead::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
			CTerrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_City"),
			CCity::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Main"),
			CCamera_Main::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ForkLift"),
			CForkLift::Create(m_pDevice, m_pContext))))
			return E_FAIL;


		///* For.Prototype_GameObject_Monster */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Droid"),
			CMonster_Droid::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TankHihumi"),
			CMonster_Boss::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		lstrcpy(m_szLoadingText, TEXT("ex obj."));

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Aru_Ex"),
			CAru_Ex::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Mutsuki_Ex"),
			CMutsuki_Ex::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Kayoko_Ex"),
			CKayoko_Ex::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Haruka_Ex"),
			CHaruka_Ex::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Warning"),
			CWarning::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WarningBar"),
			CWarning_Bar::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_VictoryBg"),
			CVictory_Bg::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Victory"),
			CVictory::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Muzzle"),
			CMuzzle::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Muzzle"),
			CMonster_Muzzle::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"),
			CBullet::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Bullet"),
			CMonster_Bullet::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Smoke"),
			CSmoke::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SmokeL"),
			CSmokeL::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Flare"),
			CFlare::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cylinder"),
			CCylinder::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BossHp"),
			CBoss_HP::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		///* For.Prototype_GameObject_Effect */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
		//	CEffect::Create(m_pGraphic_Device))))
		//	return E_FAIL;


#pragma endregion

		g_bPlay = false;
	}

		lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_CostGauge"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Cost_Gauge%d.png"), 2))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Warning"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Warning/Warning%d.png"), 3))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Victory"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Victory%d.png"), 2))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Muzzle"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Fx/Muzzle%d.png"), 5))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Smoke"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Fx/Smoke%d.jpeg"), 3))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Flare"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Fx/Flare%d.png"), 2))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_EXP"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Fx/EXP%d.png"), 3))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Damage"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Num.png"), 1))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Wave"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Fx/123.png"), 1))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Hp"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/hp.png"), 1))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Ex_Sky"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Ex_Sky/Ex%d.dds"), 4))))
			return E_FAIL;

		lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

		_matrix			TransformMatrix;

		/* For.Prototype_Component_Model_ForkLift*/
		TransformMatrix = XMMatrixScaling(0.7f, 0.7f, 0.7f) * XMMatrixRotationY(XMConvertToRadians(0.f));

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Cover/", "cover.fbx", TransformMatrix))))
			return E_FAIL;

		ZeroMemory(&TransformMatrix, sizeof(_matrix));
		TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_City"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/map/", "City.fbx", TransformMatrix))))
			return E_FAIL;

		ZeroMemory(&TransformMatrix, sizeof(_matrix));
		TransformMatrix = XMMatrixRotationX(XMConvertToRadians(0.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Droid"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Droid/", "Droid.fbx", TransformMatrix))))
			return E_FAIL;

		ZeroMemory(&TransformMatrix, sizeof(_matrix));
		TransformMatrix = XMMatrixRotationX(XMConvertToRadians(90.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TankHihumi"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Boss/", "Boss.fbx", TransformMatrix))))
			return E_FAIL;

		//fx
		ZeroMemory(&TransformMatrix, sizeof(_matrix));
		TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Cylinder0"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fx/", "Cylinder0.obj", TransformMatrix))))
			return E_FAIL;



		lstrcpy(m_szLoadingText, TEXT("EX모델을 로딩중. "));

		for (_uint i = 0; i < pSensei->Get_FormationInfoSize(); ++i)
		{
			switch (pSensei->Get_FormationInfo(i).eStudent)
			{
			case ARU:
				ZeroMemory(&TransformMatrix, sizeof(_matrix));
				TransformMatrix = XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Aru_Ex"),
					CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Aru/", "Aru_Ex.fbx", TransformMatrix))))
					return E_FAIL;
				break;

			case MUTSUKI:
				ZeroMemory(&TransformMatrix, sizeof(_matrix));
				TransformMatrix = XMMatrixRotationX(XMConvertToRadians(0.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Mutsuki_Ex"),
					CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Mutsuki/", "Mutsuki_Ex.fbx", TransformMatrix))))
					return E_FAIL;
				break;

			case KAYOKO:
				ZeroMemory(&TransformMatrix, sizeof(_matrix));
				TransformMatrix = XMMatrixRotationX(XMConvertToRadians(0.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Kayoko_Ex"),
					CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Kayoko/", "Kayoko_Ex.fbx", TransformMatrix))))
					return E_FAIL;
				break;

			case HARUKA:
				ZeroMemory(&TransformMatrix, sizeof(_matrix));
				TransformMatrix = XMMatrixRotationX(XMConvertToRadians(0.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Haruka_Ex"),
					CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Student/Haruka/", "Haruka_Ex.fbx", TransformMatrix))))
					return E_FAIL;
				break;

			}
		}
		

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
