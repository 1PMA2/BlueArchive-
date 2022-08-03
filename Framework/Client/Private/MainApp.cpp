#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"


CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.isWindowMode = GRAPHICDESC::MODE_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;	

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	m_pGameInstance->Tick_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance/* || 
		nullptr == m_pRenderer*/)
		return E_FAIL;

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.5f, 0.5f, 0.5f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();
	
	// m_pRenderer->Draw_RenderGroup();

	m_pGameInstance->Render_Engine();

	m_pGameInstance->Present();

	return S_OK;
}


HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	/*if (nullptr == m_pGameInstance)
		return E_FAIL;

	CLevel_Loading*		pLevel_Loading = CLevel_Loading::Create(m_pGraphic_Device, eLevelID);
	if (nullptr == pLevel_Loading)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, pLevel_Loading)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	///* For.Prototype_Component_Renderer */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
	//	m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_Component_VIBuffer_Rect*/
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
	//	CVIBuffer_Rect::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_Component_Transform */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
	//	CTransform::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_Component_Shader_Rect */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Rect"),
	//	CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl")))))
	//	return E_FAIL;

	//Safe_AddRef(m_pRenderer);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MessageBox(g_hWnd, TEXT("Failed to Created : CMainApp"), TEXT("System Error"), MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);		

	CGameInstance::Release_Engine();	
}