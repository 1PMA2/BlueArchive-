#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{

#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXNORTEX);

	m_iNumVertices = iNumVerticesX * iNumVerticesZ;

	m_iNumVertexBuffers = 1;


	m_pVerticesPos = new _float4[m_iNumVertices];
	VTXNORTEX*			pVertices = new VTXNORTEX[m_iNumVertices];

	for (_uint i = 0; i < iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX; ++j)
		{
		

			m_pVerticesPos[i * iNumVerticesX + j] = _float4((_float)j, 0.f, (_float)i, 1.f);
			pVertices[i * iNumVerticesX + j].vPosition = _float3((_float)j, 0.f, (_float)i);
			pVertices[i * iNumVerticesX + j].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[i * iNumVerticesX + j].vTexUV = _float2((_float)j / (_float)(iNumVerticesX - 1.f), (_float)i / (_float)(iNumVerticesZ - 1.f));
		}
	}

#pragma endregion

#pragma region INDEXBUFFER
	m_iIndicesStride = sizeof(FACEINDICES32);
	m_iNumPrimitive = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];

	FACEINDICES32*		pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	_ulong		dwNumFaces = 0;

	for (_uint i = 0; i < iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX - 1; ++j)
		{
			_uint	iIndex = i * iNumVerticesX + j;

			_uint	iIndices[] = {
				iIndex + iNumVerticesX,
				iIndex + iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[dwNumFaces]._1 = iIndices[0];
			pIndices[dwNumFaces]._2 = iIndices[1];
			pIndices[dwNumFaces]._3 = iIndices[2];

			_vector		vSour, vDest, vNormal;

			vSour = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal) + vNormal);
			++dwNumFaces;

			pIndices[dwNumFaces]._1 = iIndices[0];
			pIndices[dwNumFaces]._2 = iIndices[2];
			pIndices[dwNumFaces]._3 = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal) + vNormal);
			++dwNumFaces;
		}
	}
	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumVertices; ++i)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

#pragma endregion

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
	Safe_Delete_Array(pVertices);

	return S_OK;
}

//HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
//{
//	_ulong			dwByte = 0;
//	HANDLE			hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//	if (0 == hFile)
//		return E_FAIL;
//
//	BITMAPFILEHEADER			fh;
//	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
//
//	BITMAPINFOHEADER			ih;
//	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
//	m_iNumVerticesX = ih.biWidth;
//	m_iNumVerticesZ = ih.biHeight;
//
//	_ulong*			pPixel = new _ulong[ih.biWidth * ih.biHeight];
//	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);
//
//	CloseHandle(hFile);
//
//#pragma region VERTEXBUFFER
//	m_iStride = sizeof(VTXNORTEX);
//
//
//	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
//
//	m_iNumVertexBuffers = 1;
//
//
//	m_pVerticesPos = new _float4[m_iNumVertices];
//	VTXNORTEX*			pVertices = new VTXNORTEX[m_iNumVertices];
//
//	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
//	{
//		for (_uint j = 0; j < m_iNumVerticesX; ++j)
//		{
//			_uint	iIndex = i * m_iNumVerticesX + j;
//
//			m_pVerticesPos[iIndex] = _float4(j, 0, i, 1.f);
//			pVertices[iIndex].vPosition = _float3(j, 0, i);
//			pVertices[iIndex].vNormal = _float3(0.0f, 0.0f, 0.f);
//			pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
//		}
//	}	
//
//
//
//
//	Safe_Delete_Array(pPixel);
//#pragma endregion
//
//#pragma region INDEXBUFFER
//	m_iIndicesStride = sizeof(FACEINDICES32);
//	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
//	m_iNumIndices = 3 * m_iNumPrimitive;
//	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
//	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
//
//	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
//
//	FACEINDICES32*		pIndices = new FACEINDICES32[m_iNumPrimitive];
//	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);
//
//	_ulong		dwNumFaces = 0;
//
//	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
//	{
//		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
//		{
//			_uint	iIndex = i * m_iNumVerticesX + j;
//
//			_uint	iIndices[] = {
//				iIndex + m_iNumVerticesX, 				
//				iIndex + m_iNumVerticesX + 1,
//				iIndex + 1, 
//				iIndex
//			};
//
//			pIndices[dwNumFaces]._1 = iIndices[0];
//			pIndices[dwNumFaces]._2 = iIndices[1];
//			pIndices[dwNumFaces]._3 = iIndices[2];
//
//			_vector		vSour, vDest, vNormal;
//
//			vSour = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vPosition);
//			vDest = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition);
//			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));
//
//			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal, 
//				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal) + vNormal);
//			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal,
//				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal) + vNormal);
//			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal,
//				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal) + vNormal);
//			++dwNumFaces;
//
//			pIndices[dwNumFaces]._1 = iIndices[0];
//			pIndices[dwNumFaces]._2 = iIndices[2];
//			pIndices[dwNumFaces]._3 = iIndices[3];
//
//			vSour = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vPosition);
//			vDest = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition);
//			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));
//
//			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal,
//				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal) + vNormal);
//			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal,
//				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal) + vNormal);
//			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal,
//				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal) + vNormal);
//			++dwNumFaces;
//		}
//	}
//	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);
//
//	for (_uint i = 0; i < m_iNumVertices; ++i)
//		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
//
//#pragma endregion
//
//	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
//	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	m_BufferDesc.StructureByteStride = 0;
//	m_BufferDesc.CPUAccessFlags = 0;
//	m_BufferDesc.MiscFlags = 0;
//
//
//	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	m_SubResourceData.pSysMem = pIndices;
//
//	if (FAILED(__super::Create_IndexBuffer()))
//		return E_FAIL;
//
//	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
//	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	m_BufferDesc.StructureByteStride = m_iStride;
//	m_BufferDesc.CPUAccessFlags = 0;
//	m_BufferDesc.MiscFlags = 0;
//
//	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	m_SubResourceData.pSysMem = pVertices;
//
//	if (FAILED(__super::Create_VertexBuffer()))
//		return E_FAIL;
//
//	Safe_Delete_Array(pIndices);
//	Safe_Delete_Array(pVertices);
//
//	return S_OK;
//}

HRESULT CVIBuffer_Terrain::Initialize(void * pArg)
{
	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

//CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* pHeightMapFilePath)
//{
//	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath)))
//	{
//		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

CComponent* CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
}
