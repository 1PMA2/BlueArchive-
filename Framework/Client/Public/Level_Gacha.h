#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Gacha final : public CLevel
{
public:
	CLevel_Gacha(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Gacha() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual void Late_Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Student(const _tchar* pLayerTag);

private:
	void Gacha();

private:
	class CLoader*	m_pLoader = nullptr;

public:
	static CLevel_Gacha* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END