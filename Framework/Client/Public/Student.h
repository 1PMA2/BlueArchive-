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

class CStudent abstract : public CGameObject
{
	friend class CSensei;
public:
	typedef struct tagStudentDesc
	{
		_tchar*	pName;
		STUDENT eStudent;
		WEAPON eWeapon;
		FORMATION eFormation;

		_bool	bExModel;
		_uint	iHp;
		_uint	iShield;
		_uint	iAtk;
		_uint	iDef; //엄폐시 데미지 입을 확률
		_uint	iEx; //스킬공격력, 실드, 힐 여러곳에서 사용
		_float	fExCost;

		_uint	iMagazine;
		_int	iBullet;
		_float	fRange; //공격범위
		_float	fReConRange; //인식범위

		_float	fFireSpeed;

		ANIM	eAnim;

	}STUDENTINFO;

public:
	CStudent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStudent(const CStudent& rhs);
	virtual ~CStudent() = default;

public:
	STUDENTINFO Get_StudentInfo() { return m_tStudentInfo; }
	_vector		Get_StudentTranslation() { return m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION); }
	void		Use_Bullet() { m_tStudentInfo.iBullet -= 1; }
	void		Reload() { m_tStudentInfo.iBullet = m_tStudentInfo.iMagazine; }
	void		Set_State(ANIM eANIM) { m_tStudentInfo.eAnim = eANIM; }
	void		Set_Formation(FORMATION eFormation) { m_tStudentInfo.eFormation = eFormation; }
	_bool		Get_IsColl() { return m_bColled; }
	_bool		Get_Other() { return m_bOther; }
	void		Set_Other(_bool bOther) { m_bOther = bOther; }
	CStudent*   Get_OtherStudent() { return m_pOtherStudent; }
	_bool		Get_ExReady() { return m_bExReady; }
	void		Set_ExReady(_bool bEx) { m_bExReady = bEx; }
	//void		Set_ExReady () { m_bExReady = (m_bExReady) ? (false) : (true); }
	_bool		Get_Ex() { return m_bEx; }
	void		Set_Ex(_bool bEx) { m_bEx = bEx; }
	_bool		Is_Retire() { return m_bRetire; }
	void		Set_Retire(_bool bRetire) { m_bRetire = bRetire; }

	CMonster*		FoundMonster();

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	

protected:
	STUDENTINFO				m_tStudentInfo = {};
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;

	CCollider*				m_pPickedCom = nullptr;
	CState*					m_pState = nullptr;
	_bool					m_bPicked = false;
	CMonster*				m_pTargetMonster = nullptr;
	_float					m_fMin = 9999.f;

	CTransform*				m_pTargetTransform = nullptr;
	CTransform*				m_pSlideTransform = nullptr;
	_float					m_fTimeAcc = 0.f;
	_bool					m_bColled = false;
	_bool					m_bOther = false;
	CStudent*				m_pOtherStudent = nullptr;
	_bool					m_bExReady = false;
	_bool					m_bEx = false;
	_vector					m_vPreTranslation = {};

	_bool					m_bDamaged = false;
	_bool					m_bRetire = false;

	vector<CMonster*>	m_Monsters;
	typedef vector<CMonster*> MONSTERS;

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	virtual void OnDisable();
	virtual void OnEnable();

protected:
	virtual HRESULT FormationLevel_Collision(_float fTimeDelta);
	virtual HRESULT GamePlayLevel_Collision(_float fTimeDelta);

private:
	void InitializeStudentState();
	void ToStudentCollision(_float fTimeDelta);



public:
	virtual void Free() override;
	//virtual CGameObject* Clone(void* pArg) { return nullptr; }

};

END