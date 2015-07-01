#ifndef __ZUI__H__
#define __ZUI__H__

#include <stdint.h>

#ifdef WIN32
	#ifdef __cplusplus
		#define ZAPI(Type) extern "C" __declspec(dllexport) Type __stdcall
	#else
		#define ZAPI(Type) __declspec(dllexport) Type __stdcall
	#endif
#else
	#define ZAPI(Type) extern "C" Type __attribute__((__stdcall__))
#endif

#define ARGB(A,R,G,B) ((uint32_t)((((A)&0xff)<<24)|(((R)&0xff)<<16)|(((G)&0xff)<<8)|((B)&0xff)))
#define RGBA(R,G,B,A) ARGB(A,R,G,B)
#define RGB2ARGB(COLOR,A) RGBA(((COLOR) >> 16 & 0xFF), ((COLOR) >> 8 & 0xFF), ((COLOR) & 0xFF), (A))
#ifdef LINUX
#define RGB(r,g,b) ((VanillaInt)(((VanillaByte)(r)|((short)((VanillaByte)(g))<<8))|(((VanillaInt)(VanillaByte)(b))<<16)))
#endif



typedef wchar_t*	ZuiText, _ZuiText;	//内核默认Unicode存储字符
typedef float		ZuiReal;
#ifdef _WIN64
	typedef int64_t		ZuiInt;
#else
	typedef int32_t		ZuiInt;
#endif
typedef int			ZuiBool;
typedef void		ZuiVoid;
typedef uint32_t	ZuiColor;
typedef unsigned	ZuiByte;
typedef unsigned int ZuiTimer;
typedef void*		ZuiAny;
typedef short		ZuitShort;
typedef struct _ZRect *ZuiRect, ZRect;
typedef struct _ZPoint *ZuiPoint, ZPoint;
typedef struct _ZSize *ZuiSize, ZSize;
typedef struct _ZRectR *ZuiRectR, ZRectR;
typedef struct _ZPointR *ZuiPointR, ZPointR;
typedef struct _ZSizeR *ZuiSizeR, ZSizeR;

typedef struct _ZGraphics		*ZuiGraphics,		ZGraphics;
typedef struct _ZStringFormat	*ZuiStringFormat,	ZStringFromat;
typedef struct _ZImage			*ZuiImage, ZImage;
typedef struct _ZWindow			*ZuiWindow, ZWindow;
typedef struct _ZOsWindow		*ZuiOsWindow, ZOsWindow;
typedef struct _ZControl			*ZuiControl, ZControl;


typedef int(*ZCtlProc)(ZuiControl ID, ZuiInt Message, ZuiInt Param1, ZuiInt Param2);
typedef int(*ZCtlEventProc)(ZuiWindow Window, ZuiControl Control, ZuiInt Event, ZuiInt Param1, ZuiInt Param2, ZuiInt Param3);

#define VM_CREATE		1	//创建
#define VM_DESTROY		2	//销毁
#define VM_SETFOCUS		3	//得到焦点
#define VM_KILLFOCUS	4	//失去焦点
#define VM_SETVISIBLE	5
#define VM_SETENABLED	6
#define VM_PAINT		7	//绘制
#define VM_SIZE			8	//大小被改变
#define VM_MOVE			9	//位置被移动
#define VM_TIMER		10	//时钟
#define VM_CHAR			11	//字符输入
#define VM_KEYDOWN		12	//按下某键
#define VM_KEYUP		13	//弹起某键
#define VM_LBUTTONDOWN	14	//鼠标左键按下
#define VM_LBUTTONUP	15	//鼠标左键弹起
#define VM_LBUTTONCLK	16	//鼠标左键单击
#define VM_LBUTTONDBCLK	17	//鼠标左键双击
#define VM_RBUTTONDOWN	18	//鼠标右键按下
#define VM_RBUTTONUP	19	//鼠标右键弹起
#define VM_RBUTTONCLK	20	//鼠标右键单击
#define VM_RBUTTONDBCLK	21	//鼠标右键双击
#define VM_MBUTTONDOWN	101
#define VM_MBUTTONUP	102
#define VM_MBUTTONCLK	103
#define VM_MBUTTONDBCLK	104
#define VM_MOUSEWHEEL	22
#define VM_SETPARENT	24
#define VM_MOUSEMOVE	26	//鼠标移动
#define VM_MOUSEIN		27	//鼠标进入
#define VM_MOUSEOUT		28	//鼠标移出
#define VM_REDRAW		29
#define VM_UPDATE		30
#define VM_WNDMSG		300	//未知窗口消息
#define VM_USER			301
#define VM_NOTIFY		999


#define VE_LBTNCLK		1	// 左键单击
#define VE_LBTNDOWN		2	// 左键按下
#define VE_LBTNUP		3	// 左键弹起
#define VE_LBTNDBCLK	4	// 左键双击
#define VE_RBTNCLK		5	// 右键单击
#define VE_RBTNDOWN		6	// 右键按下
#define VE_RBTNUP		7	// 右键弹起
#define VE_RBTNDBCLK	8	// 右键双击
#define VE_KEYDOWN		9	// 按下某键
#define VE_KEYUP		10	// 放开某键
#define VE_CHAR			11	// 字符输入
#define VE_MOUSEMOVE	12	// 鼠标移动
#define VE_MOUSEIN		13	// 鼠标进入
#define VE_MOUSELEAVE	14	// 鼠标离开
#define VE_MOUSEWHEEL	15	// 滚轮滚动
#define VE_MBTNCLK		16	// 中键单击
#define VE_MBTNDOWN		17	// 中键按下
#define VE_MBTNUP		18	// 中键弹起
#define VE_MBTNDBCLK	19	// 中键双击






typedef struct _ZRect
{
	ZuiInt Left;	//左边
	ZuiInt Top;		//顶边
	ZuiInt Width;	//宽度
	ZuiInt Height;	//高度
} *ZuiRect, ZRect;
#define MAKEVRECT(r, L, T, W, H) \
	r.Left = L; \
	r.Top = T;\
	r.Width = W;\
	r.Height = H;
#define MAKEVRECTP(r, L, T, W, H) \
	r->Left = L; \
	r->Top = T;\
	r->Width = W;\
	r->Height = H;
typedef struct _ZPoint
{
	ZuiInt x;
	ZuiInt y;
} *ZuiPoint, ZPoint;
#define MAKEVPOINT(p, x1, y1) \
	p.x = x1; \
	p.y = y1;
#define MAKEVPOINTP(p, x1, y1) \
	p->x = x1; \
	p->y = y1;
typedef struct _ZSize
{
	ZuiInt Width;	//宽度
	ZuiInt Height;	//高度
} *ZuiSize, ZSize;
#define MAKEVSIZE(s, W, H) \
	s.Width = W; \
	s.Height = H;
#define MAKEVSIZEP(s, W, H) \
	s->Width = W; \
	s->Height = H;
typedef struct _ZRectR
{
	ZuiReal Left;
	ZuiReal Top;
	ZuiReal Width;
	ZuiReal Height;
} *ZuiRectR, ZRectR;

typedef struct _ZPointR
{
	ZuiReal x;
	ZuiReal y;
} *ZuiPointR, ZPointR;

typedef struct _ZSizeR
{
	ZuiReal Width;
	ZuiReal Height;
} *ZuiSizeR, ZSizeR;


typedef struct _ZBin
{
	ZuiByte* Address;
	ZuiInt Length;
} *ZuiBin, ZBin;
#define MAKEVBIN(b, ptr, len) \
	b.Address = ptr; \
	b.Length = len;
#define MAKEVBINP(b, ptr, len) \
	b->Address = ptr; \
	b->Length = len; 
#define MALLOCVBIN() ((ZuiBin)malloc(sizeof(VBin)))
#define FREEVBIN(b) \
	free(b);


#include "core\Control.h"
#include "core\window.h"
#include "core\function.h"
/*控件*/

typedef struct _ZButton			*ZuiButton, ZButton;
#include "control\ZuiButton.h"
typedef struct _ZPlayList			*ZuiPlayList, ZPlayList;
#include "../list/play_list.h"

#endif