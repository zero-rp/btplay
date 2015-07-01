#ifndef __ZUI_CORE_WINDOW_H__
#define __ZUI_CORE_WINDOW_H__
#include "../zui.h"
#include "../platform/platform.h"

#include "Control.h"
#include "../lua/lua.h"

#define VWS_POSMIDDLE		1   //居中
#define VWS_CTLBTN_LEFT		256	//
#define VWS_CTLBTN_RIGHT	512	//
#define VWS_CTLBTN_CLOSE	2   //控制按钮_关闭
#define VWS_CATBTN_MAX		4   //控制按钮_最大化
#define VWS_CTLBTN_MIN		8   //控制按钮_最小化
#define VWS_NOTASKBAR		16  //不在任务栏显示
#define VWS_TITLE			32  //标题栏
#define VWS_DRAG_TITLE		0   //拖动标题栏(默认)
#define VWS_DRAG_ANY		128 //拖动任意
#define VWS_DRAG_NO			64  //拖动禁止


#define VBT_LEFTTOP			1	//居左上
#define VBT_TENSILE			2	//拉伸
#define VBT_NOCOLORFILL		4	//无填充颜色

#define VWFS_RECT			0	//矩形
#define VWFS_ROUNDRECT		1	//圆角矩形
#define VWFS_CUSTOM			2	//

#define VWDC_ROOT  0	//窗口根控件
#define VWDC_TITLE 1	//标题控件
#define VWDC_ICON  2	//图标控件

typedef struct _ZWindowDefaultControl
{
	ZuiControl Control;
	ZuiWindow Window;
	ZuiInt ID;
} *ZuiWindowDefaultControl, ZWindowDefaultControl;

typedef struct _ZWindow
{
	ZuiOsWindow OsWindow;				//移植层窗口
	ZRect Rect;							//窗口矩形 此矩形保存窗口相对于桌面的位置
	ZuiByte Alpha;						//透明度
	//ZuiTaskQueue TaskQueue;				//任务列队
	ZuiInt Shape;						//形状
	ZuiInt ShadowColor;					//阴影颜色

	ZuiBool DragType;					//随意拖动
	ZuiBool SizeTunable;				//尺寸可调

	ZuiGraphics GraphicsBackground;		//背景图形
	ZuiGraphics GraphicsWindow;			//窗口图形

	ZuiImage BackgroundImage;			//背景图片
	ZuiColor BackgroundColor;			//背景颜色
	ZuiInt BackgroundType;				//背景类型

	ZWindowDefaultControl RootControl;		//根控件 整个窗口都为根控件
	/*标题栏数据*/
	ZWindowDefaultControl Title;			//标题栏控件
	ZuiStringFormat Title_StringFormat;	//标题文本格式
	ZuiText Title_Title;				//标题

	ZuiControl ButtonDownControl[3];	//
	ZuiControl MouseInControl;			//鼠标所在的控件
	ZuiControl FocusControl;			//焦点控件
	ZuiControl CaptureControl;			//俘获输入控件

	ZuiBool FirstShow;					//可视


	lua_State *L;						//本窗口绑定的lua虚拟机
} *ZuiWindow, ZWindow;

/**
* 此函数用作创建ZuiWindow对象.
* @param WndStyle 窗口风格
* @param Title 窗口标题
* @param StringFormat 文本格式
* @param BackgroundImage 背景图像
* @param BackgroundColor 背景颜色
* @param BackgroundType 背景类型
* @param Shape 形状 VWFS_开头的常量
* @param Returns 成功返回ZuiWindow对象,不成功返回NULL.
*/
ZAPI(ZuiWindow) ZuiCreateWindow(ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height,
	ZuiInt WndStyle,
	ZuiText Title,
	ZuiStringFormat StringFormat,
	ZuiImage BackgroundImage,
	ZuiColor BackgroundColor,
	ZuiInt BackgroundType,
	ZuiInt Shape
	);
/**
* 此函数用作销毁ZuiWindow对象.
* @param Window ZuiWindow对象
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiDestroyWindow(ZuiWindow Window);
/**
& 此函数用作设置窗口可视状态.
* @param Window ZuiWindow对象
* @param Visible 是否可视
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiSetWindowVisible(ZuiWindow Window, ZuiBool Visible);
/**
* 此函数用作获取窗口当前的可视状态.
* @param Window ZuiWindow对象
* @param Returns 当前的可视状态.
*/
ZAPI(ZuiBool) ZuiGetWindowVisible(ZuiWindow Window);
/**
* 此函数用作设置窗口大小是否可调.
* @param Window ZuiWindow对象
* @param Tunable 是否可调
* @param Returns 当前的可视状态.
*/
ZAPI(ZuiVoid) ZuiSetSizeTunable(ZuiWindow Window, ZuiBool Tunable);
/**
* 此函数用作获取窗口大小是否可调.
* @param Window ZuiWindow对象
* @param Returns 当前是否可调.
*/
ZAPI(ZuiBool) ZuiGetSizeTunable(ZuiWindow Window, ZuiBool Tunable);
/**
* 此函数用作获取窗口的根控件.
* @param Window ZuiWindow对象
* @param Returns 该窗口的根控件.
*/
ZAPI(ZuiControl) ZuiGetWindowRootControl(ZuiWindow Window);
/**
* 此函数用作设置窗口背景.
* @param Window ZuiWindow对象
* @param BackkgroundImage 背景图片
* @param BackkgroundColor 背景颜色
* @param BackkgroundType 背景类型
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiSetWindowBkg(ZuiWindow Window, ZuiImage BackkgroundImage, ZuiColor BackkgroundColor, ZuiInt BackkgroundType);
/**
* 此函数用作设置窗口形状.
* @param Window ZuiWindow对象
* @param Shape 形状 VWFS_开头的常量
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiSetWindowShape(ZuiWindow Window, ZuiInt Shape);
/**
* 此函数用作获取窗口当前形状.
* @param Window ZuiWindow对象
* @param Returns 返回当前形状.
*/
ZAPI(ZuiInt) ZuiGetWindowShape(ZuiWindow Window);
/**
* 此函数用作设置窗口阴影颜色.
* 使用函数请先设置分层窗口
* @param Window ZuiWindow对象
* @param ShadowColor  阴影颜色
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiSetWindowShadowColor(ZuiWindow Window, ZuiInt ShadowColor);
/**
* 此函数用作获取窗口阴影颜色.
* @param Window ZuiWindow对象
* @param Returns 阴影颜色
*/
ZAPI(ZuiInt) ZuiGetWindowShadowColor(ZuiWindow Window);
/**
* 此函数用作重画并刷新窗口缓存图形.
* @param Window ZuiWindow对象
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiRedrawWindowGraphics(ZuiWindow Window);
/**
* 此函数用作重画并刷新窗口.
* @param Window ZuiWindow对象
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiRedrawWindow(ZuiWindow Window);
/**
* 此函数用作刷新窗口.
* @param Window ZuiWindow对象
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiFlashWindow(ZuiWindow Window);
/**
* 此函数用作设置窗口开启混合(半透明)效果.
* 此函数在Linux下无效,因为Linux的窗口总是自动开启混合效果
* @param Window ZuiWindow对象
* @param Composite 是否开启混合
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiSetWindowComposite(ZuiWindow Window, ZuiBool Composite);
/**
* 此函数用作活取窗口是否开启混合(半透明)效果.
* 此函数在Linux下无效,返回值将总为true,因为Linux的窗口总是自动开启混合效果
* @param Window ZuiWindow对象
* @param Returns 当前混合状态
*/
ZAPI(ZuiBool) ZuiGetWindowComposite(ZuiWindow Window);
/**
* 此函数用作设置窗口透明度.
* @param Window ZuiWindow对象
* @param Alpha 透明度 0-255
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiSetWindowAlpha(ZuiWindow Window, ZuiByte Alpha);
/**
* 此函数用作获取窗口当前透明度.
* @param Window ZuiWindow对象
* @param Returns 返回当前透明度.
*/
ZAPI(ZuiByte) ZuiGetWindowAlpha(ZuiWindow Window);



ZuiVoid ZuiWindowInitGraphics(ZuiWindow Window, ZuiBool ForceRecreate);
ZuiVoid ZuiWindowDrawWindow(ZuiWindow Window, ZuiGraphics Graphics);
ZuiVoid ZuiWindowDrawBackground(ZuiWindow Window, ZuiGraphics Graphics);
ZuiVoid ZuiWindowDrawBackgroundImage(ZuiWindow Window, ZuiGraphics Graphics);
ZuiVoid ZuiWindowDrawControl(ZuiWindow Window);
ZuiVoid ZuiWindowUpdate(ZuiWindow Window, ZuiRect UpdateRect);
ZuiVoid ZuiWindowUpdateGraphicsRect(ZuiWindow Window, ZuiRect UpdateRect, ZuiBool ForceRedraw, ZuiBool Flash);

ZuiInt ZuiWindowDefaultControlsProc(ZuiControl Control, ZuiInt Message, ZuiInt Param1, ZuiInt Param2);
#endif //__ZUI_CORE_WINDOW_H__