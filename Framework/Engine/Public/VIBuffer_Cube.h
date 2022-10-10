#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cube final : public CVIBuffer
{
protected:
	CVIBuffer_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Cube(const CVIBuffer_Cube& rhs);
	virtual ~CVIBuffer_Cube() = default;

public:
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(((FACEINDICES16*)m_pIndices)[iIndex]._1,
			((FACEINDICES16*)m_pIndices)[iIndex]._2,
			((FACEINDICES16*)m_pIndices)[iIndex]._3);
	}

public:
	virtual HRESULT Initialize_Prototype(_float fSize = 1.f);
	virtual HRESULT Initialize(void* pArg) override;

private:
	_float m_fSize;

public:
	static CVIBuffer_Cube* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _float fSize);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END