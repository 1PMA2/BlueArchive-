#pragma once

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

class CCamera_Ex final : public CCamera
{
public:
	typedef struct tagExCameraDesc
	{
		LEVEL			eTargetLevel;
		const _tchar*	pTargetLayerTag;
		_uint			iTargetIndex;
		const _tchar*	pTargetModelComTag;

		const char*		pBoneName;
	}EXCAMERADESC;

public:
	CCamera_Ex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Ex(const CCamera_Ex& rhs);
	virtual ~CCamera_Ex() = default;

public:
	_bool Get_IsEx() { return m_bIsEx; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	EXCAMERADESC				m_ExDesc;
	_float4x4				m_WorldMatrix;
	_float4x4				m_TargetWorldMatrix;

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

private:
	class CHierarchyNode*	m_pBonePtr = nullptr;
	class CHierarchyNode*	m_pTargetBonePtr = nullptr;
	class CTransform*		m_pTargetTransform = nullptr;
	_bool	m_bIsEx = false;

private:
	void MoveCamera(_float fTimeDelta);


public:
	static CCamera_Ex* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END