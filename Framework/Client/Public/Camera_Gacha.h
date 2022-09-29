#include "Client_Defines.h"
#include "GameObject.h"
#include "Camera.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CHierarchyNode;
END

BEGIN(Client)

class CCamera_Gacha final : public CCamera
{
public:
	typedef struct tagWeaponDesc
	{
		LEVEL			eTargetLevel;
		const _tchar*	pTargetLayerTag;
		_uint			iTargetIndex;
		const _tchar*	pTargetModelComTag;

		const char*		pBoneName;
	}WEAPONDESC;

public:
	CCamera_Gacha(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Gacha(const CCamera_Gacha& rhs);
	virtual ~CCamera_Gacha() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	WEAPONDESC				m_WeaponDesc;
	_float4x4				m_WorldMatrix;
	_float4x4				m_TargetWorldMatrix;

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	class CHierarchyNode*	m_pBonePtr = nullptr;
	class CHierarchyNode*	m_pTargetBonePtr = nullptr;
	class CTransform*		m_pTargetTransform = nullptr;
	_float m_fFov = 10.f;
	_float m_fOpenFov = 4.3f;
	_bool m_bOnce = true;

private:
	void MoveCamera();


public:
	static CCamera_Gacha* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END