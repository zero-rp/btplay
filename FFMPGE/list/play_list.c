#include "play_list.h"
/*按钮控件的回调函数*/
ZuiInt ZuiPlayListProc(ZuiControl Control, ZuiInt Message, ZuiInt Param1, ZuiInt Param2) {
	ZuiButton p = (ZuiButton)Control->ControlData;
	ZuiGraphics Graphics;
	switch (Message) {
	case VM_LBUTTONDOWN:
		ZuiControlRedraw(Control, TRUE);
		break;
	case VM_LBUTTONUP:
		ZuiControlRedraw(Control, TRUE);
		break;
	case VM_MOUSEIN:
		ZuiControlRedraw(Control, TRUE);
		break;
	case VM_MOUSEOUT:
		ZuiControlRedraw(Control, TRUE);
		break;
	case VM_SETFOCUS:
		ZuiControlRedraw(Control, TRUE);
		//ZuiControlGradient(Control, 5, 7, 0);
		break;
	case VM_KILLFOCUS:
		ZuiControlRedraw(Control, TRUE);
		break;
	case VM_PAINT:
		Graphics = (ZuiGraphics)Param2;
			ZuiFillRect(Graphics, ARGB(255,0,0,0), 0, 0, Control->CRect.Width, Control->CRect.Height);
		break;
	}
	return 0;
}
ZAPI(ZuiControl) ZuiPlayListCreate(ZuiControl ParentControl, ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height,ZuiBool Visible, ZuiBool Enabled) {
	ZuiPlayList p = (ZuiPlayList)malloc(sizeof(ZPlayList));
	if (p){
		memset(p, 0, sizeof(ZPlayList));
		ZuiControl Control = ZuiControlCreate(ParentControl, Left, Top, Width, Height, p, Visible, Enabled, NULL);
		if (Control) {
			Control->CtlProc = &ZuiPlayListProc;

			return Control;
		}
		free(p);
	}
	return NULL;
}