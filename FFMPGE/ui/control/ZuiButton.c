// Zui按钮(控件)
#include "ZuiButton.h"
#define BUTTON_STATUS_NORMAL 0
#define BUTTON_STATUS_HOVER  1
#define BUTTON_STATUS_CLICK  2

/*按钮控件的回调函数*/
ZuiInt ZuiButtonProc(ZuiControl Control, ZuiInt Message, ZuiInt Param1, ZuiInt Param2) {
	ZuiButton p = (ZuiButton)Control->ControlData;
	ZuiGraphics Graphics;
	switch (Message) {
	case VM_LBUTTONDOWN:
		p->Status = BUTTON_STATUS_CLICK;
		//ZuiControlGradient(Control, 5, 7, 0);
		ZuiControlRedraw(Control, TRUE);
		break;
	case VM_LBUTTONUP:
		p->Status = BUTTON_STATUS_HOVER;
		//ZuiControlGradient(Control, 5, 7, 0);
		ZuiControlRedraw(Control, TRUE);
		break;
	case VM_MOUSEIN:
		p->Status = BUTTON_STATUS_HOVER;
		//ZuiControlGradient(Control, 5, 7, 0);
		ZuiControlRedraw(Control, TRUE);
		break;
	case VM_MOUSEOUT:
		p->Status = BUTTON_STATUS_NORMAL;
		//ZuiControlGradient(Control, 5, 7, 0);
		ZuiControlRedraw(Control, TRUE);
		break;
	case VM_SETFOCUS:
		ZuiControlRedraw(Control, TRUE);
		//ZuiControlGradient(Control, 5, 7, 0);
		break;
	case VM_KILLFOCUS:
		ZuiControlRedraw(Control, TRUE);
		//ZuiControlGradient(Control, 5, 7, 0);
		break;
	case VM_PAINT:
		Graphics = (ZuiGraphics)Param2;
		switch (p->Style) {
		case VBS_METRO:
			ZuiFillRect(Graphics, p->ButtonColor[ZuiControlIsEnable(Control) ? p->Status : 3], 0, 0, Control->CRect.Width, Control->CRect.Height);
			ZuiDrawString(Graphics, p->StringFormat, p->Title, &Control->CRect);
			if (Control->Window->FocusControl == Control) {
				ZuiDrawRect(Graphics, p->ButtonColor[4], 1, 1, Control->CRect.Width - 2, Control->CRect.Height - 2, 2);
			}
			else {
				ZuiDrawRect(Graphics, p->ButtonColor[4], 0, 0, Control->CRect.Width-1, Control->CRect.Height-1, 1);
			}
		}
		break;
	}
	return 0;
}

ZAPI(ZuiControl) ZuiButtonCreate(ZuiControl ParentControl, ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height, ZuiText Title, ZuiStringFormat StringFormat, ZuiBool Visible, ZuiBool Enabled) {
	ZuiButton p = (ZuiButton)malloc(sizeof(ZButton));
	if (p){
		memset(p, 0, sizeof(ZButton));
		ZuiControl Control = ZuiControlCreate(ParentControl, Left, Top, Width, Height, p, Visible, Enabled, NULL);
		if (Control) {
			Control->CtlProc = &ZuiButtonProc;
			p->Status = 0;
			p->Style = 0;
			p->StringFormat = StringFormat;
			int len = wcslen(Title);
			p->Title = (ZuiText)malloc(len * 2);
			wcscpy(p->Title, Title);
			return Control;
		}
		free(p);
	}
	return NULL;
}

ZAPI(ZuiVoid) ZuiButtonSetStyle_Metro(ZuiControl Control, ZuiColor ButtonColor [5]) {
	ZuiButton p = (ZuiButton)Control->ControlData;
	for (ZuiInt i = 0; i < 5; i++) {
		p->ButtonColor[i] = ButtonColor[i];
	}
	p->Style = VBS_METRO;
}

ZAPI(ZuiInt) ZuiButtonGetStyle(ZuiControl Control) {
	return ((ZuiButton)(Control->ControlData))->Style;
}

ZAPI(ZuiVoid) ZuiButtonSetTitle(ZuiControl Control, ZuiText Title) {
	ZuiButton p = (ZuiButton)Control->ControlData;
	if (p->Title)
	{
		free(p->Title);
	}
	int len = wcslen(Title);
	p->Title = (ZuiText)malloc(len);
	memcpy(p->Title, Title, len);
}

ZAPI(ZuiVoid) ZuiButtonSetStringFormat(ZuiControl Control, ZuiStringFormat StringFormat) {
	((ZuiButton)(Control->ControlData))->StringFormat = StringFormat;
}

ZAPI(ZuiInt) ZuiButtonGetTitle(ZuiControl Control) {
//	return String2Text(((ZuiButton)(Control->ControlData))->Title);
	return 0;
}

/*---------XML绑定------------*/
/*---------LUA绑定------------*/