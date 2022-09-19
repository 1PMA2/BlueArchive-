#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	_bool Get_isFinished() { return m_isFinished; }

public:
	HRESULT Initialize(aiAnimation* pAIAnimation, class CModel* pModel);
	void Update_TransformationMatrices(_float fTimeDelta);
	void Reset_TransformationMatrices();

public:
	HRESULT Clone_Channel(CAnimation* pPrototype, class CModel* pModel);

private:
	char			m_szName[MAX_PATH] = "";
	_uint			m_iNumChannels = 0; /* ���� �ִϸ��̼��� ��뤷�ؾ��� ���� ����. */
	vector<class CChannel*>		m_Channels; /* ���� �ִԿ��� ���Ǵ� ���� ���¸� ������ ��ü�̴�. */

	_float			m_fDuration = 0.f;
	_float			m_fTickPerSecond = 0.f;
	_float			m_fTimeAcc = 0.f;
	_bool			m_isFinished = false;



public:
	static CAnimation* Create(aiAnimation* pAIAnimation, class CModel* pModel);
	CAnimation* Clone(class CModel* pModel);
	virtual void Free() override;
};

END