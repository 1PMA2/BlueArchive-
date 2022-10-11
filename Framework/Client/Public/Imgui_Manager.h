#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "imgui.h"

BEGIN(Engine)
class CTransform;
class CGameInstance;
class CGameObject;
class CVIBuffer_Terrain;
END

BEGIN(Client)
class CImgui_Manager final : public CBase
{

	DECLARE_SINGLETON(CImgui_Manager)

private:
	CImgui_Manager();
	virtual ~CImgui_Manager() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	HRESULT Render();

	void ShowWindow(bool* p_open);

private:
	CGameInstance*			m_pGameInstance = nullptr;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

public:
	virtual void Free();
};

END