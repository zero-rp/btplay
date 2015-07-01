#ifndef __ZUI_PLATFORM_GRAPH_GDI_H__
#define __ZUI_PLATFORM_GRAPH_GDI_H__

#include "../platform.h"
#include "../../zui.h"

#if (defined PLATFORM_GRAPH_GDI) && (PLATFORM_GRAPH_GDI == 1) && (defined PLATFORM_OS_WIN)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <malloc.h>

/*图像*/
typedef struct _ZImage {
	void *image;
	ZuiInt Width;
	ZuiInt Height;
}*ZuiImage, ZImage;

/*图形*/
typedef struct _ZGraphics {
	ZuiInt Width;
	ZuiInt Height;
	void *graphics;//图形
	HDC hdc;
	HBITMAP HBitmap;
}*ZuiGraphics, ZGraphics;

/*文本格式*/
typedef struct _ZStringFormat{
	ZuiColor TextColor;
	ZuiColor ShadowColor;
	void *font;//字体
	void *StringFormat;//文本格式
	void *FontFamily;//字体族
	void *BrushShadow;
	void *Brush;
}*ZuiStringFormat, ZStringFromat;

#endif //PLATFORM_GRAPH_GDI
#endif //__ZUI_PLATFORM_GRAPH_GDI_H__