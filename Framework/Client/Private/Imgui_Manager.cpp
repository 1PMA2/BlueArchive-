#include "stdafx.h"
#include "..\Public\Imgui_Manager.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "GameObject.h"
#include "Sensei.h"

IMPLEMENT_SINGLETON(CImgui_Manager)

CImgui_Manager::CImgui_Manager()
	:m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CImgui_Manager::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	return S_OK;
}

void CImgui_Manager::Tick(_float fTimeDelta)
{
}

HRESULT CImgui_Manager::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (show_arrange_window)
	//	ImGui::ShowDemoWindow(&show_arrange_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static float fTranslationX = {};
		static float fTranslationY = {};
		static float fScaleX = 0.1f;
		static float fScaleY = 0.1f;
		static int counter = 0;
		

		ImGui::Begin(u8"µûÈåÈæ");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("costbar", &costbar);      // Edit bools storing our window open/close state
		ImGui::Checkbox("costwindow", &costbg);

		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
		ImGui::SliderFloat("X Translation", &fTranslationX, -640.f, 640.f);  // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("Y Translation", &fTranslationY, -360.f, 360.f);
		ImGui::SliderFloat("X Scale", &fScaleX, 10.f, 1280.f);  // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("Y Scale", &fScaleY, 10.f, 1280.f);

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Muzzle"));
		}

		if (0 < m_pGameInstance->Get_GameObjectSize(LEVEL_GAMEPLAY, TEXT("Layer_Effect")))
			pObj = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"));
		else
			pObj = nullptr;

		if (nullptr != pObj)
		{
			CTransform* pTransform = (CTransform*)pObj->Get_Component(TEXT("Com_Transform"));

			//pTransform->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(fTranslationX, fTranslationY, 0.f, 1.f));
			//pTransform->Set_Scaled(_float3(fScaleX, fScaleY, 0.f));
		}

		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		


		ImGui::End();
	}

	if (costbar)
	{
		pObj = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 4);
	}


	if (costbg)
	{
		pObj = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 3);
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	
	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	//const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };


	return S_OK;
}

void CImgui_Manager::ShowWindow(bool * p_open)
{
}

void CImgui_Manager::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
