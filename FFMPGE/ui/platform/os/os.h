#ifndef __ZUI_PLATFORM_OS_H__
#define __ZUI_PLATFORM_OS_H__

#include "../platform.h"

/**
* 此函数用作初始化Os.
* @param Returns 成功返回true.
*/
ZuiBool ZuiOsInitialize();
/**
* 此函数用作创建定时器.
* @param nElapse 定时周期
* @param Control 通知控件
* @param Param1 自定义参数
* @param Returns 成功返回ZuiTimer对象,不成功返回NULL.
*/
ZuiTimer ZuiOsCreateTimer(ZuiInt nElapse, ZuiControl Control, ZuiInt Param1);
/**
* 此函数用作销毁定时器.
* @param Timer 欲销毁的ZuiTimer对象
* @此函数没有返回值.
*/
ZuiVoid ZuiOsDestroyTimer(ZuiTimer Timer);
/**
* 此函数用作创建ZuiOsWindow对象.
* @param Rect 窗口矩形
* @param Title 窗口标题
* @param ShowInTaskbar 是否在任务栏显示
* @param PosMiddle 是否居中
* @param Window 框架层ZuiWindow指针
* @param Returns 成功返回ZuiOsWindow对象,不成功返回NULL.
*/
ZuiOsWindow ZuiOsCreateWindow(ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height, ZuiText Title, ZuiBool ShowInTaskbar, ZuiBool PosMiddle, ZuiWindow Window);
/**
* 此函数用作销毁由ZuiOsCreateWindow创建的对象.
* @param OsWindow ZuiOsWindow对象
* @此函数没有返回值.
*/
ZuiVoid ZuiOsDestroyWindow(ZuiOsWindow OsWindow);
/**
* 此函数用作设置窗口标题.
* @param OsWindow ZuiOsWindow对象
* @param Title 标题
* @此函数没有返回值.
*/
ZuiVoid ZuiOsSetWindowTitle(ZuiOsWindow OsWindow, ZuiText Title);
/**
* 此函数用作获取窗口标题.
* @param OsWindow ZuiOsWindow对象
* @param out 输出缓冲区
* @param outlen 缓冲区长度
* @此函数没有返回值.
*/
ZuiInt ZuiOsGetWindowTitle(ZuiOsWindow OsWindow, ZuiText out, ZuiInt outlen);
/**
* 此函数用作设置窗口可视状态.
* @param OsWindow ZuiOsWindow对象
* @param Visible 可视状态
* @此函数没有返回值.
*/
ZuiVoid ZuiOsSetWindowVisible(ZuiOsWindow OsWindow, ZuiBool Visible);
/**
* 此函数用作获取窗口当前可视状态.
* @param OsWindow ZuiOsWindow对象
* @param Returns 当前窗口可视状态.
*/
ZuiBool ZuiOsGetWindowVisible(ZuiOsWindow OsWindow);
/**
* 此函数用作设置是否开启混合效果.
* @param OsWindow ZuiOsWindow对象
* @param Composite true or false
* @此函数没有返回值.
*/
ZuiVoid ZuiOsSetWindowComposite(ZuiOsWindow OsWindow, ZuiBool Composite);
/**
* 此函数用作获取是否开启混合效果.
* @param OsWindow ZuiOsWindow对象
* @param Returns 当前混合状态.
*/
ZuiBool ZuiOsGetWindowComposite(ZuiOsWindow OsWindow);
/**
* 此函数用作拖动窗口.
* @param OsWindow ZuiOsWindow对象
* @此函数没有返回值.
*/
ZuiVoid ZuiOsDragWindow(ZuiOsWindow OsWindow);
/**
* 此函数用作刷新窗口显示.
* @param OsWindow ZuiOsWindow对象
* @param UpdateRect 刷新区域
* @此函数没有返回值.
*/
ZuiVoid ZuiOsUpdateWindow(ZuiWindow Window, ZuiRect UpdateRect);
/**
* 此函数用作设置窗口鼠标.
* @param Window ZuiOsWindow对象
* @param Cursor 鼠标类型
* @此函数没有返回值.
*/
ZuiVoid ZuiOsSetCursor(ZuiWindow Window, ZuiInt Cursor);
/**
* 此函数用作为Control捕获输入.
* @param Window ZuiWindow对象
* @此函数没有返回值.
*/
ZuiVoid ZuiOsSetCapture(ZuiWindow Window);
/**
* 此函数用作释放输入捕获.
* @此函数没有返回值.
*/
ZuiVoid ZuiOsReleaseCapture();
/**
* 此函数用作设置窗口输入法状态.
* @param Window ZuiWindow对象
* @param Status 打开或关闭
* @此函数没有返回值.
*/
ZuiVoid ZuiOsSetImeStatus(ZuiWindow Window,ZuiBool Status);
/**
* 此函数用作设置窗口输入法位置.
* @param Window ZuiWindow对象
* @param x 
* @param y
* @此函数没有返回值.
*/
ZuiVoid ZuiOsSetImeCurrentPos(ZuiWindow Window, ZuiInt x, ZuiInt y);
/**
* 此函数用作移植层的消息循环.
* @param Returns int.
*/
ZuiInt ZuiOsMessageLoop();
/**
* 此函数用作获取系统启动时间.
* @param Returns int.
*/
ZuiInt ZuiOsGetTickCount();
/**
* 此函数用作于主动检测程序运行时某个按键的状态， 及弹起或按下状态.
* @param vKey 按键的虚拟键码
* @param Returns 若返回值非0， 则返回按键的状态.
*/
ZuiBool ZuiOsGetAsyncKeyState(ZuiInt vKey);
/**
* 此函数用作将Ansi转成Unicode.

* @param Returns 如果交不为空，则返回true；否则，如果交为空则返回false.
*/
ZAPI(ZuiInt) ZuiA2W(char *str, ZuiInt slen, ZuiText out, ZuiInt olen);
/**
* 此函数用作将Unicode转成Ansi.

* @param Returns 如果交不为空，则返回true；否则，如果交为空则返回false.
*/
ZAPI(ZuiInt) ZuiW2A(ZuiText str, ZuiInt slen, char *out, ZuiInt olen);



#endif //__ZUI_PLATFORM_OS_H__