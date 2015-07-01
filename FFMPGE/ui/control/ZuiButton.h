#ifndef __Zui_CONTROLS_BUTTON_H__
#define __Zui_CONTROLS_BUTTON_H__
// Zui按钮(控件)
#include "../zui.h"

#define VBS_METRO 1
#define VBS_IMAGE 2

typedef struct _ZButton
{
	ZuiText Title;
	ZuiColor ButtonColor[5];
	ZuiImage Image[5];
	ZuiStringFormat StringFormat;
	ZuiInt Status;
	ZuiInt Style;
} *ZuiButton, ZButton;
/**
* 此函数用作创建按钮.
* @param ParentControl 父控件
* @param Left 左边
* @param Top 顶边
* @param Width 宽度
* @param Height 高度
* @param Title 标题
* @param StringFormat 文本格式
* @param Visible 可视
* @param Enabled 激活
* @param Returns 成功返回控件对象 失败返回NULL
*/
ZAPI(ZuiControl) ZuiButtonCreate(ZuiControl ParentControl, ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height, ZuiText Title, ZuiStringFormat StringFormat, ZuiBool Visible, ZuiBool Enabled);
ZAPI(ZuiVoid) ZuiButtonSetStyle_Metro(ZuiControl Control, ZuiColor ButtonColor [5]);
ZAPI(ZuiInt) ZuiButtonGetStyle(ZuiControl Control);
ZAPI(ZuiVoid) ZuiButtonSetTitle(ZuiControl Control, ZuiText Title);
ZAPI(ZuiVoid) ZuiButtonSetStringFormat(ZuiControl Control, ZuiStringFormat StringFormat);
ZAPI(ZuiInt) ZuiButtonGetTitle(ZuiControl Control);
#endif	//__Zui_CONTROLS_BUTTON_H__

