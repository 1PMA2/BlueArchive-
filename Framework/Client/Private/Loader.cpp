#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "LoadingImage.h"
#include "BG_Lobby.h"
#include "Camera_Free.h"
//#include "Monster.h"
#include "Terrain.h"
#include "Player.h"
//#include "Effect.h"
#include "Sky.h"


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
		case LEVEL_GAMEPLAY:
			hr = pLoader->Loading_ForGamePlayLevel();
			break;
		}


	if (FAILED(hr))
		MSG_BOX("Failed to Loading");

	LeaveCriticalSection(&pLoader->Get_CS());

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

	if (pGameInstance->Get_NumPlayObj() > pGameInstance->Get_PTypeSize()) //객체 개수, 재생성 막기
	{
#pragma region PROTOTYPE_GAMEOBJECT

		lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

		/* For.Prototype_GameObject_Lobby */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BG_Lobby"),
			CBG_Lobby::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		//pGameInstance->Set_NumLobbyObj(pGameInstance->Get_PTypeSize());

#pragma endregion
	}

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_BG_Lobby"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Bg_Lobby%d.png"), 1))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));



	lstrcpy(m_szLoadingText, TEXT("fhql로딩 끝 "));

	m_isFinished = true;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();


	Safe_AddRef(pGameInstance);


	if (3 >= pGameInstance->Get_PTypeSize()) //객체 개수, 재생성 막기
	{

#pragma region PROTOTYPE_GAMEOBJECT

		lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

		/* For.Prototype_GameObject_Sky */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
			CSky::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Terrain*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
			CTerrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Camera_Free*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
			CCamera_Free::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Player */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
			CPlayer::Create(m_pDevice, m_pContext))))
			return E_FAIL;


		///* For.Prototype_GameObject_Monster */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		//	CMonster::Create(m_pGraphic_Device))))
		//	return E_FAIL;

		///* For.Prototype_GameObject_Effect */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
		//	CEffect::Create(m_pGraphic_Device))))
		//	return E_FAIL;


#pragma endregion

		pGameInstance->Set_NumPlayObj(pGameInstance->Get_PTypeSize());
	}

		lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));



		/* For.Prototype_Component_Texture_Sky */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Terrain */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Filter */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Filter"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.bmp"), 1))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Brush */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
			return E_FAIL;

		///* For.Prototype_Component_Texture_Player */
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Player.jpg"), 1))))
		//	return E_FAIL;



		///* For.Prototype_Component_Texture_Monster */
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster"),
		//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Snow/Snow.png"), 1))))
		//	return E_FAIL;

		///* For.Prototype_Component_Texture_Explosion */
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Explosion"),
		//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
		//	return E_FAIL;


		lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

		/* For.Prototype_Component_VIBuffer_Terrain */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
			return E_FAIL;

		/* For.Prototype_Component_VIBuffer_Cube*/
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
			CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
			return E_FAIL;


		_matrix			TransformMatrix;

		/* For.Prototype_Component_Model_ForkLift*/
		TransformMatrix = XMMatrixScaling(1.01f, 1.01f, 1.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ForkLift/", "ForkLift.fbx", TransformMatrix))))
			return E_FAIL;


		/* For.Prototype_Component_Model_Fiona */
		//TransformMatrix = /*XMMatrixScaling(0.01f, 0.01f, 0.01f) * */XMMatrixRotationY(XMConvertToRadians(180.0f));

		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fiona/", "Fiona.fbx", TransformMatrix))))
		//	return E_FAIL;

		lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중이빈다. "));
		/* For.Prototype_Component_Shader_VtxTex */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxCubeTex */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeTex"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxModel */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements))))
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
