#ifndef __ZUI_PLATFORM_OS_WIN_H__
#define __ZUI_PLATFORM_OS_WIN_H__

#include "../platform.h"
#include "../../zui.h"
#if (defined PLATFORM_OS_WIN)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Imm.h>
#define PROP_ID		(LPCWSTR)101
#define PROP_OLDPROC	(LPCWSTR)102

typedef struct _ZOsWindow
{
	HWND hWnd;				//窗口句柄
	HDC hDC;				//
	int Layered;			//分层窗口
	HIMC   hIMC;			//输入法句柄
} *ZuiOsWindow, ZOsWindow;
#endif //PLATFORM_OS_WIN
#endif //__ZUI_PLATFORM_OS_WIN_H__