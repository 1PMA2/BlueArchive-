#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CLevel;

class CEvent_Manager final
	: public CBase
{
	DECLARE_SINGLETON(CEvent_Manager);

public:
	CEvent_Manager();
	virtual ~CEvent_Manager() = default;

private:
	enum EVENT_ID {
		EVENT_DELETE_OBJECT, EVENT_CREATE_OBJECT, EVENT_CREATE_STATIC, EVENT_CHANGE_LEVEL,
		EVENT_DISABLE_OBJECT, EVENT_ENABLE_OBJECT, EVENT_END
	};

	typedef struct tag_Event_Info
	{
		EVENT_ID		eEven;
		DWORD_PTR		lParam;
		DWORD_PTR		wParam;

		tag_Event_Info(EVENT_ID _eEven, DWORD_PTR _lParam, DWORD_PTR _wParam)
			: eEven(_eEven)
			, lParam(_lParam)
			, wParam(_wParam)
		{}
	}EVENT;


public:
	void	Tick();

public:
	void	Delete_GameObject(CGameObject*	pGameObject);
	void	Create_GameObject(CGameObject*	pGameObject);
	void	Enable_GameObject(CGameObject*	pGameObject);
	void	Disable_GameObject(CGameObject*	pGameObject);
	void	Change_Level(_uint iLevelID, CLevel * pLevel);

private:
	void	Add_Event(const EVENT_ID& eEven, const DWORD_PTR& lParam = 0, const DWORD_PTR& wParam = 0);
	void	Execute(const EVENT& tEvent);
private:
	vector<EVENT>			m_vecEvent;
	vector<CGameObject*>	m_vecDead;

public:
	virtual void Free() override;
};

END;

