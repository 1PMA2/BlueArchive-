#include "..\Public\VIBuffer_RectX.h"

CVIBuffer_RectX::CVIBuffer_RectX(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_RectX::CVIBuffer_RectX(const CVIBuffer_RectX & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_RectX::Initialize_Prototype()
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	m_pVerticesPos = new _float4[m_iNumVertices];

	VTXTEX*			pVertices = new VTXTEX[m_iNumVertices];

	m_pVerticesPos[0] = _float4(0.f, 0.5f, -0.5f, 1.f);
	pVertices[0].vPosition = _float3(0.f, 0.5f, -0.5f);
	pVertices[0].vTexUV = _float2(0.0f, 0.f);

	m_pVerticesPos[1] = _float4(0.f, 0.5f, 0.5f, 1.f);
	pVertices[1].vPosition = _float3(0.f, 0.5f, 0.5f);
	pVertices[1].vTexUV = _float2(1.0f, 0.f);

	m_pVerticesPos[2] = _float4(0.f, -0.5f, 0.5f, 1.f);
	pVertices[2].vPosition = _float3(0.f, -0.5f, 0.5f);
	pVertices[2].vTexUV = _float2(1.0f, 1.f);

	m_pVerticesPos[3] = _float4(0.f, -0.5f, -0.5f, 1.f);
	pVertices[3].vPosition = _float3(0.f, -0.5f, -0.5f);
	pVertices[3].vTexUV = _float2(0.0f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEXBUFFER


#pragma endregion
	m_iIndicesStride = sizeof(FACEINDICES16);
	m_iNumPrimitive = 2;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	m_pIndices = new FACEINDICES16[m_iNumPrimitive];

	FACEINDICES16*		pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_RectX::Initialize(void * pArg)
{
	return S_OK;
}

CVIBuffer_RectX * CVIBuffer_RectX::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_RectX*		pInstance = new CVIBuffer_RectX(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_RectX");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_RectX::Clone(void * pArg)
{
	CVIBuffer_RectX*		pInstance = new CVIBuffer_RectX(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_RectX");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RectX::Free()
{
	__super::Free();
}
