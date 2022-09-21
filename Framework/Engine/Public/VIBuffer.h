#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	_uint Get_NumPrimitive() const {
		return m_iNumPrimitive;
	}

	_float4* Get_VerticesPos() const {
		return m_pVerticesPos;
	}

	virtual _uint3 Get_Indices(_uint iIndex) const = 0;

	_uint Get_IndexSize() {
		return m_eIndexFormat == DXGI_FORMAT_R16_UINT ? 2 : 4;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Render();

protected:
	D3D11_BUFFER_DESC				m_BufferDesc;
	D3D11_SUBRESOURCE_DATA			m_SubResourceData;

protected:
	ID3D11Buffer*					m_pVB = nullptr;
	ID3D11Buffer*					m_pIB = nullptr;
	_uint							m_iStride = 0;
	_uint							m_iNumVertices = 0;
	_uint							m_iNumPrimitive = 0;
	_uint							m_iIndicesStride = 0;
	_uint							m_iNumIndices = 0;
	_uint							m_iNumVertexBuffers = 0;
	DXGI_FORMAT						m_eIndexFormat;
	D3D11_PRIMITIVE_TOPOLOGY		m_eToplogy;
	_float4*						m_pVerticesPos = nullptr;

protected:
	void* m_pIndices = nullptr;
	


protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();



public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END