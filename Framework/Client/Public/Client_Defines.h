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
	enum ANIM {ANIM_EXCUTIN, ANIM_EX, ANIM_SET, ANIM_SETUP, ANIM_ZOOM, ANIM_END};
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

extern bool g_bLobby;
extern bool g_bPlay;

using namespace Client;