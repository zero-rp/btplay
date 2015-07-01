#ifndef __ZUI_CORE_CONTROL_H__
#define __ZUI_CORE_CONTROL_H__
#include "../zui.h"
#include "../platform/platform.h"
#include "window.h"
typedef struct _ZControl
{
	// 链表 Begin
	// 同级控件
	ZuiControl LastControl;		//上一个
	ZuiControl NextControl;		//下一个
	// 子控件
	ZuiControl ChildControlFirst;//第一个
	ZuiControl ChildControlEnd;	//最后一个
	// 上级控件
	ZuiControl ParentControl;
	// 链表 End

	// 控件属性 Begin
	ZuiWindow Window;			//控件所在的窗口
	ZuiBool Visible;			//可视
	ZuiBool Enabled;			//可用

	ZuiInt DisabledCount;		// 父层所有组件中不可用的层数
	ZuiInt InvisibleCount;		// 父层所有组件中不可视的层数
	ZuiBool MousePenetration;	// 鼠标穿透
	ZuiByte Alpha;				// 透明度
	ZCtlProc CtlProc;			// 控件回调函数 控件内部回调
	ZCtlEventProc EventProc;	// 事件回调函数 控件用户回调
	ZuiControl BindOwner;		// 绑定控件 此控件可接收到NOTIFY消息

	/*暂时不知道什么用*/
	ZuiBool Focusable;			//可否获取焦点
	ZuiBool Virtual;			//虚拟控件 不分配图形

	ZRect Rect;			//矩形区域 相对于父窗口的
	ZRect CRect;		//矩形区域 客户区的

	// 缓存图形
	ZuiGraphics Graphics;
	// 渐变缓存图形
	ZuiGraphics Graphics_Gradient1;
	ZuiGraphics Graphics_Gradient2;
	// 渐变参数
	ZuiBool Gradienting;		//渐变完成
	ZuiReal GradientAlpha;		//渐变透明度
	ZuiInt GradientUserData;
	ZuiBool GradientType;
	ZuiTimer GradientTimer;		//渐变时钟
	// 控件属性 End

	void *ControlData;
} *ZuiControl, ZControl;

/**
* 此函数用作创建ZuiControl(控件)对象.
* @param ParentControl 上级控件
* @param Left 左边
* @param Top 顶边
* @param Width 宽度
* @param Height 高度
* @param ControlData 用户数据
* @param Visible 可视
* @param Enabled 激活
* @param CreateStruct
* @param Returns 成功返回ZuiControl对象,不成功返回NULL.
*/
ZAPI(ZuiControl) ZuiControlCreate(ZuiControl ParentControl, ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height, ZuiAny ControlData, ZuiBool Visible, ZuiBool Enabled, ZuiAny CreateStruct);
/**
* 此函数用作销毁ZuiControl(控件)对象.
* @param Control ZuiControl对象
* @param Returns 返回释放的控件总数.
*/
ZAPI(ZuiInt) ZuiControlDestroy(ZuiControl Control);
/**
* 此函数用作重画ZuiControl(控件)对象.
* @param Control ZuiControl对象
* @param Update 是否刷新
* @param Returns 返回控件处理重绘事件后的返回值(一般为0).
*/
ZAPI(ZuiInt) ZuiControlRedraw(ZuiControl Control, ZuiBool Update);
/**
* 此函数用作渐变更新ZuiControl(控件)对象.
* 渐变更新,类内部使用.调用本方法后,OnPaint事件被触发,然后借助时钟绘制出渐变效果(将会使用时钟ID:-1),注意,本方法是用异步实现的,而且无法终止
* @param Control ZuiControl对象
* @param dwTime 每两次更新间隔毫秒数,默认为10
* @param dwGradient 每两次更新间渐变幅度,默认为10
* @param bType 真:在旧的缓存图形上慢慢增加不透明度覆盖绘制新的图像(模拟SyserUI工作方式)|||假:慢慢增加新图像的不透明度,减少就图像的不透明度(模拟Ex_DirectUI2.0工作方式)
* @param Returns 返回控件处理重绘事件后的返回值(一般为0).
*/
ZAPI(ZuiVoid) ZuiControlGradient(ZuiControl Control, ZuiInt dwTime, ZuiInt dwGradient, ZuiBool bType);
/**
* 此函数用作设置Control可用状态.
* @param Control ZuiControl对象
* @param Visible 是否可用
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiControlSetEnable(ZuiControl Control, ZuiBool Enabled);
/**
* 此函数用作获取Control可用状态.
* @param Control ZuiControl对象
* @param Returns 返回Control可用状态.
*/
ZAPI(ZuiBool) ZuiControlIsEnable(ZuiControl Control);
/**
* 此函数用作设置Control可视状态.
* @param Control ZuiControl对象
* @param Visible 是否可视
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiControlSetVisible(ZuiControl Control, ZuiBool Visible);
/**
* 此函数用作获取Control可视状态.
* @param Control ZuiControl对象
* @param Returns 返回Control是否可视.
*/
ZAPI(ZuiBool) ZuiControlIsVisible(ZuiControl Control);
/**
* 此函数用作设置Control透明度.
* @param Control ZuiControl对象
* @param Alpha 透明度
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiControlSetAlpha(ZuiControl Control, ZuiByte Alpha);
/**
* 此函数用作获取Control透明度.
* @param Control ZuiControl对象
* @param Returns 返回Control透明度.
*/
ZAPI(ZuiByte) ZuiControlGetAlpha(ZuiControl Control);
/**
* 此函数用作设Control置焦点.
* @param Control ZuiControl对象
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiControlSetFocus(ZuiControl Control);
/**
* 此函数用作获取Control焦点状态.
* @param Control ZuiControl对象
* @param Returns 返回Control焦点状态.
*/
ZAPI(ZuiBool) ZuiControlGetFocus(ZuiControl Control);
/**
* 此函数用作为Control捕获输入.
* @param Control ZuiControl对象
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiControlSetCapture(ZuiControl Control);

ZAPI(ZuiVoid) ZuiControlReleaseCapture(ZuiControl Control);
/**
* 此函数用作移动Control.
* @param Control ZuiControl对象
* @param Left 左边
* @param Top 顶边
* @param Width 宽度
* @param Height 高度
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiControlMove(ZuiControl Control, ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height);
/**
* 此函数用作获取Control相对于窗口根控件(RootControl)的区域.
* @param Control ZuiControl对象
* @param Rect 用来存放结果的Rect对象
* @param Returns 返回&Rect
*/
ZAPI(ZuiRect) ZuiControlGetRectOfWindow(ZuiControl Control, ZuiRect Rect);
/**
* 此函数用作获取Control矩形区域.
* @param Control ZuiControl对象
* @param Returns 返回Control矩形
*/
ZAPI(ZuiRect) ZuiControlGetRect(ZuiControl Control);
/**
* 此函数用作设置Control事件回调函数.
* @param Control ZuiControl对象
* @param EventProc 回调函数
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiControlSetEventProc(ZuiControl Control, ZCtlEventProc EventProc);
/**
* 此函数用作获取Control事件回调函数指针.
* @param Control ZuiControl对象
* @param Returns 返回回调函数指针
*/
ZAPI(ZCtlEventProc) ZuiControlGetEventProc(ZuiControl Control);
/**
* 此函数用作设置全局默认事件回调函数.
* @param EventProc 全局默认事件回调函数
* @此函数没有返回值.
*/
ZAPI(ZuiVoid) ZuiControlSetDefaultEventProc(ZCtlEventProc EventProc);
/**
* 此函数用作触发Control的事件.
* @param Control ZuiControl对象
* @param Event 事件类型
* @param Returns 返回用户处理事件后的返回值
*/
ZAPI(ZuiInt) ZuiControlTriggerEvent(ZuiControl Control, ZuiInt Event, ZuiInt Param1, ZuiInt Param2, ZuiInt Param3);
/**
* 此函数用作给Control发送消息.
* @param Control ZuiControl对象
* @param Message 事件类型
* @param Param1 参数1
* @param Param2 参数2
* @param Returns 返回用户处理事件后的返回值
*/
ZAPI(ZuiInt) ZuiControlSendMessage(ZuiControl Control, ZuiInt Message, ZuiInt Param1, ZuiInt Param2);
ZAPI(ZuiVoid) ZuiControlSendMessageToChild(ZuiControl ParentControl, ZuiInt Message, ZuiInt Param1, ZuiInt Param2);
ZAPI(ZuiVoid) ZuiControlSendMessageToChildOfWindow(ZuiWindow Window, ZuiInt Message, ZuiInt Param1, ZuiInt Param2);
/**
* 此函数用作根据点在一个窗口内寻找另外一个控件.
* @param Window ZuiWindow对象
* @param x
* @param y
* @param x1 Out 相对坐标
* @param y1 Out
* @param Returns 返回寻找到的Control对象
*/
ZAPI(ZuiControl) ZuiFindControlInWindow(ZuiWindow Window, ZuiInt x, ZuiInt y, ZuiInt *x1, ZuiInt *y1);
/**
* 此函数用作根据点在一个控件内寻找另外一个控件.
* @param ParentControl ZuiControl对象
* @param x
* @param y
* @param x1 Out 相对坐标
* @param y1 Out
* @param Returns 返回寻找到的Control对象
*/
ZAPI(ZuiControl) ZuiFindControlInControl(ZuiControl ParentControl, ZuiInt x, ZuiInt y, ZuiInt *x1, ZuiInt *y1);
/**
* 控件的默认处理函数
*/
ZuiInt ZuiDefaultControlProc(ZuiControl Control, ZuiInt Message, ZuiInt Param1, ZuiInt Param2);



#endif //__ZUI_CORE_CONTROL_H__