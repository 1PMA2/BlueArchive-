#include "stdafx.h"
#include "..\Public\UI_Student.h"

#include "GameInstance.h"


CUI_Student::CUI_Student(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{

}

CUI_Student::CUI_Student(const CUI_Student & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_Student::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CUI_Student::Initialize(void * pArg)
{
	return E_NOTIMPL;
}

void CUI_Student::Tick(_float fTimeDelta)
{
}

void CUI_Student::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_Student::Render()
{
	return E_NOTIMPL;
}

HRESULT CUI_Student::SetUp_Components()
{
	return E_NOTIMPL;
}

HRESULT CUI_Student::SetUp_ShaderResource()
{
	return E_NOTIMPL;
}

void CUI_Student::OnDisable()
{
}

void CUI_Student::OnEnable()
{
}

void CUI_Student::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
