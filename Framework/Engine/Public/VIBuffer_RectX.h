#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectX final : public CVIBuffer
{
protected:
	CVIBuffer_RectX(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_RectX(const CVIBuffer_RectX& rhs);
	virtual ~CVIBuffer_RectX() = default;

public:
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(((FACEINDICES16*)m_pIndices)[iIndex]._1,
			((FACEINDICES16*)m_pIndices)[iIndex]._2,
			((FACEINDICES16*)m_pIndices)[iIndex]._3);
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_RectX* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END