#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
END

BEGIN(Client)

class CMonster_State;

class CMonster abstract : public CGameObject
{
public:
	typedef struct tagMonsterDesc
	{
		MONSTER eMonster;

		_int iHp;
		_int iAtk;
		_float fRange;

	}MONSTERINFO;

public:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	MONSTER Get_Monster() { return m_tMonsterInfo.eMonster; }
	_vector Get_MonsterTranslation() { return m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION); }
	void Set_MinusHp(_int iAtk) { m_tMonsterInfo.iHp -= (_int)(iAtk * frandom(0.9, 1.1)); }
	_bool Get_Fear() { return m_bFear; }
	void Set_Fear(_bool bFear) { m_bFear = bFear; }
	class CStudent* Get_FoundStudent() { return m_pTargetStudent; }
	class CStudent* Get_RandomStudent();
	CStudent* Get_RandomTarget() { return m_pRandomStudent; }
	_uint Get_StudentSize() { return m_Students.size(); }
	_vector Get_TargetTranslation() { return m_vTargetTranslation; }
	_vector Get_RandomTargetTranslation() { return m_vRandomTarget; }
	

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	vector<CStudent*>	m_Students;
	typedef vector<CStudent*> STUDENTS;
	CStudent* m_pTargetStudent = nullptr;
	CStudent* m_pRandomStudent = nullptr;
	_float m_fMin = 9999.f;

	CTransform::TRANSFORMDESC		m_tTransformDesc;
	MONSTERINFO				m_tMonsterInfo = {};
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;
	CMonster_State*			m_pState = nullptr;
	_bool					m_bFear = false;
	_float					m_fFearAcc = 0.f;
	_vector					m_vTargetTranslation = {};
	_vector					m_vRandomTarget = {};

public:
	virtual void OnDisable() override;
	virtual void OnEnable() override;

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();
	class CStudent*		FoundStudent();

protected:
	void SelectMonster();
	void DeleteMonster();


public:
	virtual void Free() override;
};

END
