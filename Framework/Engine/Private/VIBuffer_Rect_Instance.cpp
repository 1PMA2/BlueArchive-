#include "..\Public\VIBuffer_Rect_Instance.h"

CVIBuffer_Rect_Instance::CVIBuffer_Rect_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Rect_Instance::CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance & rhs)
	: CVIBuffer(rhs)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_iNumInstance(rhs.m_iNumInstance)
	, m_iInstanceStride(rhs.m_iInstanceStride)
	, m_pInstanceSpeeds(rhs.m_pInstanceSpeeds)
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Rect_Instance::Initialize_Prototype(_uint iNumInstance)
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_iNumVertexBuffers = 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXTEX*			pVertices = new VTXTEX[m_iNumVertices];

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.0f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(1.0f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(1.0f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _float2(0.0f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);


#pragma endregion


#pragma region INSTANCEBUFFER
	m_iInstanceStride = sizeof(VTXINSTANCE);
	m_iNumInstance = iNumInstance;


	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXINSTANCE*		pInstance = new VTXINSTANCE[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstance[i].vRight = _float4(0.f, 0.f, 1.f, 0.f);
		pInstance[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pInstance[i].vLook = _float4(1.f, 0.f, 0.f, 0.f);
		pInstance[i].vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstance;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstance);

	/* 각 인스턴스들이 드랍되어야할 스피드를 각기 다르게 보관한다. */
	//m_pInstanceSpeeds = new _float[m_iNumInstance];

	//for (_uint i = 0; i < m_iNumInstance; ++i)
	//{
	//	m_pInstanceSpeeds[i] = rand() % 7 + 5.0f;
	//}


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

	FACEINDICES16*		pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Rect_Instance::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Rect_Instance::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	ID3D11Buffer*			pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint		iStrides[] = {
		m_iStride,
		m_iInstanceStride
	};

	_uint		iOffsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eToplogy);

	/* 6 : 하나의 도형을 그리기위해 사용하는 인덱스의 갯수. 네모라서 여섯개.  */
	m_pContext->DrawIndexedInstanced(6, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Rect_Instance::Update(_float fTimeDelta)
{
	//if (nullptr == m_pContext ||
	//	nullptr == m_pVBInstance)
	//	return;

	//D3D11_MAPPED_SUBRESOURCE		SubResource;

	///* D3D11_MAP_WRITE_NO_OVERWRITE : SubResource구조체가 받아온 pData에 유요한 값이 담겨잇는 형태로 얻어오낟. */
	///* D3D11_MAP_WRITE_DISCARD : SubResource구조체가 받아온 pData에 값이 초기화된 형태로 얻어오낟. */
	//m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	//for (_uint i = 0; i < m_iNumInstance; ++i)
	//{

	//	// *(((VTXINSTANCE*)SubResource.pData) + i)
	//	((VTXINSTANCE*)SubResource.pData)[i].vTranslation.y -= m_pInstanceSpeeds[i] * fTimeDelta;

	//	if (0.0f >= ((VTXINSTANCE*)SubResource.pData)[i].vTranslation.y)
	//		((VTXINSTANCE*)SubResource.pData)[i].vTranslation.y = 10.f;

	//}

	//m_pContext->Unmap(m_pVBInstance, 0);

}

CVIBuffer_Rect_Instance * CVIBuffer_Rect_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iNumInstance)
{
	CVIBuffer_Rect_Instance*		pInstance = new CVIBuffer_Rect_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumInstance)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Rect_Instance::Clone(void * pArg)
{
	CVIBuffer_Rect_Instance*		pInstance = new CVIBuffer_Rect_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Rect_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pInstanceSpeeds);

	Safe_Release(m_pVBInstance);
}
