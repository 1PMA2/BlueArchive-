#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Timer_Manager.h"
#include "Key_Manager.h"
#include "PipeLine.h"
#include "Font_Manager.h"
#include "Light_Manager.h"
#include "Picking.h"
#include "Event_Manager.h"
#include "Target_Manager.h"

/* 1. 게임내에 필요한 객체(매니져등)들을 모아서 보관한다. */
/* 2. 클라이언트 개발자가 접근하기좋은 루트를 제공해준다. 나. */
/* 3. 진짜 필요한 함수만 클라잉언트개발자에게 ㅇ오픈해주낟. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
public:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* 엔진 라이브럴를 사용할때 필요한 초기화 과정으르 거칠거야. */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHICDESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT Tick_Engine(_float fTimeDelta);
	HRESULT Render_Engine();
	HRESULT Clear(_uint iLevelID);

public: /* For.Graphic_Device */	
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();


public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte byKeyID);
	_byte Get_DIMouseKeyState(MOUSEBUTTON eMouseButtonID);
	_long Get_DIMouseMoveState(MOUSEMOVE eMouseMove);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelID, class CLevel* pLevel);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex = 0);
	_uint Get_GameObjectSize(_uint iLevelIndex, const _tchar* pLayerTag);
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);

public: /* For.Component_Mananger */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

	
public: /* For.Timer_Manager */
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float Compute_Timer(const _tchar* pTimerTag, _float fTimeSpeed = 1.f);

public: /* For. Key_Manager */
	KEY_STATE	Get_KeyState(KEY _key);

public: /* For.PipeLine */
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformState);
	_matrix Get_Transform(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_TP(CPipeLine::TRANSFORMSTATE eState);
	_float4 Get_CamPosition();

public: /* For.Font_Manager */
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor);

	
public: /* For.Light_Manager */
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC & LightDesc);
	const LIGHTDESC* Get_LightDesc(_uint iIndex);

public: /* For.Event_Manager */
	void	Delete_GameObject(CGameObject*	pGameObject);
	void	Enable_GameObject(CGameObject*	pGameObject);
	void	Disable_GameObject(CGameObject*	pGameObject);
	void	Change_Level(_uint iLevelID, CLevel * pLevel);


public: /* For.Picking */
	_float4 Get_RayPos() { return m_pPicking->Get_RayPos(); }
	_float4 Get_RayDir() { return m_pPicking->Get_RayDir(); }
	_bool Picking(CVIBuffer* pVIBuffer, CTransform* pTransform, _float4* pOut);


public: /* For.Target_Manager */
	ID3D11ShaderResourceView* Get_RenderTarget_SRV(const _tchar* pTargetTag);








private:
	CGraphic_Device*				m_pGraphic_Device = nullptr;
	CInput_Device*					m_pInput_Device = nullptr;
	CLevel_Manager*					m_pLevel_Manager = nullptr;
	CObject_Manager*				m_pObject_Manager = nullptr;
	CComponent_Manager*				m_pComponent_Manager = nullptr;
	CTimer_Manager*					m_pTimer_Manager = nullptr;
	CKey_Manager*					m_pKey_Manager = nullptr;
	CPipeLine*						m_pPipeLine = nullptr;
	CFont_Manager*					m_pFont_Manager = nullptr;
	CLight_Manager*					m_pLight_Manager = nullptr;
	CEvent_Manager*					m_pEvent_Manager = nullptr;
	CPicking*						m_pPicking = nullptr;
	CTarget_Manager*				m_pTarget_Manager = nullptr;


public:
	static void Release_Engine();
	virtual void Free() override;
};

END