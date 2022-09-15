#pragma once
#include "Engine_Defines.h"

namespace Client 
{
	static const unsigned int g_iWinCX = 1280;
	static const unsigned int g_iWinCY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_LOBBY, LEVEL_GAMEPLAY, LEVEL_END };
	enum CAMERA { CAMERA_FREE, CAMERA_EX, CAMERA_END };
	enum FORMATION {FORMATION_FIRST, FORMATION_SECOND, FORMATION_THIRD, FORMATION_FOURTH, FORMATION_END};
	enum UNIT_STATE {UNIT_IDLE, UNIT_END};
	enum ANIM {
		ANIM_EX, ANIM_EXCUTIN, ANIM_FORMATIONIDLE, ANIM_FORMATIONPICK, ANIM_KNEEZOOM, ANIM_HIDERELOADSTART, ANIM_FIRE, ANIM_HIDEFIRESTART, ANIM_HIDESIGN, ANIM_HIDEIDLE, ANIM_HIDERELOADING, ANIM_RUNSIGN, ANIM_RUNTOHIDE, ANIM_RUNTOKNEE,
		ANIM_RUN, ANIM_KNEEZOOM2, ANIM_KNEEZOOMEND, ANIM_KNEEZOOMFIRE, ANIM_KNEEZOOMSTART, ANIM_KNEESIGN, ANIM_KNEEIDLE, ANIM_KNEERELOAD, ANIM_KNEESKILL, ANIM_VICTORY, ANIM_VICTORYSTART, ANIM_DEAD, ANIM_FAIL, ANIM_KNEEFAIL, ANIM_RETIRE, ANIM_END
	};
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

extern bool g_bLobby;
extern bool g_bPlay;

using namespace Client;