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

class CState;
class CMonster;
class CStudent : public CGameObject
{	
	friend class CSensei;
public:
	typedef struct tagStudentDesc
	{
		_tchar*	pName;

		FORMATION eFormation;

		_bool	bExModel;
		_uint	iIndex;
		_uint	iHp;
		_uint	iShield;
		_uint	iAtk;
		_uint	iDef; //엄폐시 데미지 입을 확률
		_uint	iEx; //스킬공격력, 실드, 힐 여러곳에서 사용
		_float	fExCost;

		_uint	iMagazine;
		_int	iBullet;
		_uint	iRange;

		_float	fFireSpeed;

		ANIM	eAnim;

	}STUDENTINFO;

public:
	CStudent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStudent(const CStudent& rhs);
	virtual ~CStudent() = default;

public:
	STUDENTINFO Get_StudentInfo() { return m_tStudentInfo; }
	void		Use_Bullet() { m_tStudentInfo.iBullet -= 1; }
	void		Reload() { m_tStudentInfo.iBullet = m_tStudentInfo.iMagazine; }
	void		Set_State(ANIM eANIM) { m_tStudentInfo.eAnim = eANIM; }
	void		Set_Formation(FORMATION eFormation) { m_tStudentInfo.eFormation = eFormation; }

	void		Reset_Monsters() { m_Monsters.clear(); }
	void		In_RangeMonsters(CMonster* pMonster) { m_Monsters.push_back(pMonster); }
	_uint		Get_InRangeMonsters() { return (_uint)m_Monsters.size(); }
	CMonster*		Get_InRangeMonster(_uint iIndex) { return m_Monsters.at((size_t)iIndex); }


	_bool		Is_Picked() { return m_bPicked; }
	CMonster*		FoundMonster();
	_bool		FoundObstacle() { return m_bFoundObstacle; }

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	

protected:
	class CAru*					m_pAru = nullptr;
	class CMutsuki*				m_pMustuki = nullptr;

	STUDENTINFO				m_tStudentInfo = {};
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;
	CCollider*				m_pSphereCom_Obstacle = nullptr;
	CCollider*				m_pPickedCom = nullptr;
	CState*					m_pState = nullptr;
	_bool					m_bPicked = false;
	CMonster*				m_pTargetMonster = nullptr;
	_float					m_fMin = 9999.f;

	_bool					m_bFoundObstacle = false;
	_bool					m_bSetted = false;
	_vector					m_vPreTranslation = {};

	vector<CMonster*>	m_Monsters;
	typedef vector<CMonster*> MONSTERS;

	//vector<class CCover*>	m_Covers;
	//typedef vector<CCover*> COVERS;

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	virtual void OnDisable() {}
	virtual void OnEnable() {}

protected:
	virtual HRESULT FormationLevel_Collision();
	virtual HRESULT GamePlayLevel_Collision();

private:
	void InitializeStudentState();



public:
	virtual void Free() override;
	virtual CGameObject* Clone(void* pArg) { return nullptr; }

};

END