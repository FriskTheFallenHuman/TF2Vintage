//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//===========================================================================//

#ifndef BASEMODUI_H
#define BASEMODUI_H
#ifdef _WIN32
#pragma once
#endif

#include "basemodpanel.h"
#include "basemodframe.h"
#include "GameUI_Interface.h"
#include "const.h"

#ifndef _CERT
	extern int UI_IsDebug();
#else
#	define UI_IsDebug()		0
#endif

#endif // BASEMODUI_H