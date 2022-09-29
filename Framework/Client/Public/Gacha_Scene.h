#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CGacha_Scene final : public CLevel
{
public:
	CGacha_Scene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CGacha_Scene() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual void Late_Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Arona(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Student(const _tchar* pLayerTag);


public:
	static CGacha_Scene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END