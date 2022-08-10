#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
{

}

HRESULT CShader::Initialize_Prototype(const _tchar * pShaderFilePath)
{
	_uint		iHLSLFlag = 0;

#ifdef _DEBUG
	iHLSLFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_OPTIMIZATION_LEVEL0;
#else
	iHLSLFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	/*ID3DBlob*			pError;*/

	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHLSLFlag, 0, m_pDevice, &m_pEffect, /*&pError*/nullptr)))
		return E_FAIL;

	//ID3DX11EffectPass*		pPass = nullptr;

	//pPass->Apply()

	return S_OK;
}

HRESULT CShader::Initialize(void * pArg)
{
	return S_OK;
}

CShader * CShader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pShaderFilePath)
{
	CShader*		pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CShader::Clone(void * pArg)
{
	CShader*		pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

}
