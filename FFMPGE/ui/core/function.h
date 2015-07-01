#ifndef __ZUI_CORE_FUNCTION_H__
#define __ZUI_CORE_FUNCTION_H__

#include "../zui.h"

/**
* 此函数用作初始化zui.
* @param Control ZuiControl对象
* @param Visible 是否可用
* @此函数没有返回值.
*/
ZAPI(ZuiBool) ZuiInit();
/**
* 此函数用作Vanilla消息循环.
* @param Returns int.
*/
ZAPI(ZuiInt) ZuiMsgLoop();
/**
* 此函数用作检测某点是否存在与一个矩形之内.
* @param x 欲检测的点
* @param y
* @param Rect 用作检测的矩形
* @param Returns 在或不在.
*/
ZuiBool ZuiIsPointInRect(ZuiInt x, ZuiInt y, ZuiRect Rect);
ZuiBool ZuiIsPointInRectR(ZuiReal x, ZuiReal y, ZuiRectR Rect);
/**
* 此函数用作计算两个矩形的交.
* @param Rect1 源矩形
* @param Rect2 源矩形
* @param RectResult 两个源矩形相交的矩形
* @param Returns 如果交不为空，则返回true；否则，如果交为空则返回false.
*/
ZuiBool ZuiIntersectRect(ZuiRect Rect1, ZuiRect Rect2, ZuiRect RectResult);


#endif //__ZUI_CORE_FUNCTION_H__
