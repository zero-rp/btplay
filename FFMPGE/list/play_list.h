#ifndef __Zui_CONTROLS_PLAYLIST_H__
#define __Zui_CONTROLS_PLAYLIST_H__
// 播放列表
#include "../ui/zui.h"

typedef struct _ZPlayList
{
	ZuiText Title;

} *ZuiPlayList, ZPlayList;
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
ZAPI(ZuiControl) ZuiPlayListCreate(ZuiControl ParentControl, ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height, ZuiBool Visible, ZuiBool Enabled);
#endif	//__Zui_CONTROLS_BUTTON_H__