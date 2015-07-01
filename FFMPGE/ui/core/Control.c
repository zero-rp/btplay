#include "Control.h"
ZCtlEventProc DefEventProc = NULL;//全局默认回调函数


ZAPI(ZuiControl) ZuiControlCreate(ZuiControl ParentControl, ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height, ZuiAny ControlData, ZuiBool Visible, ZuiBool Enabled, ZuiAny CreateStruct) {
	if (ParentControl == NULL) {
		return NULL;
	}
	ZuiBool RootControl;
	if ((ZuiInt)ParentControl < 0) {
		RootControl = TRUE;
	}
	else {
		RootControl = FALSE;
	}

	ZuiControl Control = (ZuiControl)malloc(sizeof(ZControl));
	memset(Control, 0, sizeof(ZControl));
	Control->Focusable = TRUE;
	Control->Virtual = FALSE;

	Control->Alpha = 255;			//透明度
	Control->BindOwner = NULL;
	Control->Window = RootControl ? ((ZuiWindow)(-(ZuiInt)ParentControl)) : ParentControl->Window;	//所在的窗口
	Control->CtlProc = NULL;		//内核回调函数
	Control->Enabled = Enabled;		//可用
	Control->EventProc = NULL;		//用户回调
	//渐变参数
	Control->GradientAlpha = 0;
	Control->Gradienting = FALSE;
	Control->GradientType = FALSE;
	Control->GradientUserData = 0;

	Control->ControlData = ControlData;//继承指针
	//同级控件
	Control->LastControl = NULL;
	Control->NextControl = NULL;
	//子控件
	Control->ChildControlFirst = NULL;
	Control->ChildControlEnd = NULL;

	Control->Graphics = NULL;			//缓存图形
	Control->Graphics_Gradient1 = NULL;	//渐变缓存
	Control->Graphics_Gradient2 = NULL;
	Control->Visible = Visible;			//可视
	Control->MousePenetration = FALSE;	//鼠标穿透
	//保存控件区域
	MAKEVRECT(Control->Rect, Left, Top, Width, Height);
	MAKEVRECT(Control->CRect, 0, 0, Width - 1, Height - 1);
	Control->ParentControl = RootControl ? NULL : ParentControl;
	if (!RootControl) {
		if (Control->ParentControl->ChildControlEnd != NULL) {
			Control->ParentControl->ChildControlEnd->NextControl = Control;
			Control->LastControl = Control->ParentControl->ChildControlEnd;
		}
		Control->ParentControl->ChildControlEnd = Control;
		if (Control->ParentControl->ChildControlFirst == NULL) {
			Control->ParentControl->ChildControlFirst = Control;
		}
	}
	Control->DisabledCount = Control->ParentControl ? (Control->ParentControl->DisabledCount + ((Control->ParentControl->Enabled) ? 0 : 1)) : 0;
	Control->InvisibleCount = Control->ParentControl ? (Control->ParentControl->InvisibleCount + ((Control->ParentControl->Visible) ? 0 : 1)) : 0;
	if (!(RootControl || Control->Virtual)) {
		Control->Graphics = ZuiCreateGraphicsInMemory(Width, Height);
	}
	else {
		Control->Graphics = NULL;
	}
	ZuiControlSendMessage(Control, ZM_CREATE, 0, (ZuiInt)CreateStruct);
	return Control;
}

ZAPI(ZuiInt) ZuiControlDestroy(ZuiControl Control) {
	if (Control == NULL) {
		return 0;
	}
	ZuiControl ThisControl;
	ZuiControl NextControl = Control->ChildControlFirst;
	ZuiInt i = 0;
	while (NextControl != NULL) {
		ThisControl = NextControl;
		NextControl = NextControl->NextControl;
		i += ZuiControlDestroy(ThisControl);
	}
	/*通知控件控件已被销毁*/
	ZuiControlSendMessage(Control, ZM_DESTROY, 0, 0);
	/*销毁图形*/
	ZuiDestroyGraphics(Control->Graphics);
	ZuiDestroyGraphics(Control->Graphics_Gradient1);
	ZuiDestroyGraphics(Control->Graphics_Gradient2);
	/*移出链表中自身*/
	if (Control->ParentControl) {
		if (Control->ParentControl->ChildControlFirst == Control) {
			Control->ParentControl->ChildControlFirst = Control->NextControl;
		}
		if (Control->ParentControl->ChildControlEnd == Control) {
			Control->ParentControl->ChildControlEnd = Control->LastControl;
		}
		if (Control->LastControl != NULL) {
			Control->LastControl->NextControl = Control->NextControl;
		}
		if (Control->NextControl != NULL) {
			Control->NextControl->LastControl = Control->LastControl;
		}
	}
	if (Control->Window->ButtonDownControl[0] == Control) {
		Control->Window->ButtonDownControl[0] = NULL;
	}
	if (Control->Window->ButtonDownControl[1] == Control) {
		Control->Window->ButtonDownControl[1] = NULL;
	}
	if (Control->Window->ButtonDownControl[2] == Control) {
		Control->Window->ButtonDownControl[2] = NULL;
	}
	if (Control->Window->CaptureControl == Control) {
		/*当前俘获输入控件是欲销毁的控件*/
		Control->Window->CaptureControl = NULL;
	}
	if (Control->Window->FocusControl == Control) {
		/*当前焦点控件是欲销毁的控件*/
		Control->Window->FocusControl = NULL;
	}
	if (Control->Window->MouseInControl == Control) {
		/*鼠标当前所在控件是欲销毁的控件*/
		Control->Window->MouseInControl = NULL;
	}
	free(Control);
	return i + 1;
}

ZAPI(ZuiInt) ZuiControlRedraw(ZuiControl Control, ZuiBool Update) {
	return ZuiControlSendMessage(Control, ZM_REDRAW, (ZuiInt)Update, 0);
}
/*渐变更新*/
ZAPI(ZuiVoid) ZuiControlGradient(ZuiControl Control, ZuiInt dwTime, ZuiInt dwGradient, ZuiBool bType)
{//真:在旧的缓存图形上慢慢增加不透明度覆盖绘制新的图像(模拟SyserUI工作方式)|||假:慢慢增加新图像的不透明度,减少就图像的不透明度(模拟Ex_DirectUI2.0工作方式)
	if (dwTime <= 0){ dwTime = 10; }
	if (dwGradient <= 0){ dwGradient = 10; }
	/*第一次渐变 初始化缓冲图形*/
	if (Control->Graphics_Gradient1 == 0){ Control->Graphics_Gradient1 = ZuiCreateGraphicsInMemory(Control->CRect.Width, Control->CRect.Height); }
	if (Control->Graphics_Gradient2 == 0){ Control->Graphics_Gradient2 = ZuiCreateGraphicsInMemory(Control->CRect.Width, Control->CRect.Height); }
	if (Control->Gradienting)
	{
		ZuiAlphaBlend(Control->Graphics, 0, 0, Control->CRect.Width, Control->CRect.Height, Control->Graphics_Gradient1, 0, 0, Control->Alpha);
	}
	/*保存旧图形到缓存2*/
	ZuiAlphaBlend(Control->Graphics_Gradient2, 0, 0, Control->CRect.Width, Control->CRect.Height, Control->Graphics, 0, 0, Control->Alpha);
	/*重绘到渐变缓存1*/
	ZuiGraphicsClear(Control->Graphics_Gradient1, 0);
	Control->CtlProc(Control, ZM_PAINT, 0, (ZuiInt)Control->Graphics_Gradient1);
	/*创建时钟*/
	if (Control->Gradienting)
	{
		//上次渐变未完成
		Control->GradientAlpha = 0;
	}
	else
	{
		Control->GradientTimer = ZuiOsCreateTimer(dwTime, Control, dwGradient);
	}

	Control->Gradienting = TRUE;
}

ZAPI(ZuiVoid) ZuiControlSetEnable(ZuiControl Control, ZuiBool Enabled) {
	if (Control->Enabled != Enabled) {
		ZuiControlSendMessageToChild(Control, ZM_SETENABLED, 0, Enabled ? -1 : 1);
		Control->Enabled = Enabled;
	}
}

ZAPI(ZuiBool) ZuiControlIsEnable(ZuiControl Control) {
	return Control->DisabledCount == 0 && Control->Enabled;
}

ZAPI(ZuiVoid) ZuiControlSetVisible(ZuiControl Control, ZuiBool Visible) {
	if (Control->Visible != Visible) {
		ZuiControlSendMessageToChild(Control, ZM_SETVISIBLE, 0, Visible ? -1 : 1);
		Control->Visible = Visible;
	}
}

ZAPI(ZuiBool) ZuiControlIsVisible(ZuiControl Control) {
	return Control->InvisibleCount == 0 && Control->Visible;
}

ZAPI(ZuiVoid) ZuiControlSetAlpha(ZuiControl Control, ZuiByte Alpha) {
	Control->Alpha = Alpha;
}

ZAPI(ZuiByte) ZuiControlGetAlpha(ZuiControl Control) {
	return Control->Alpha;
}

ZAPI(ZuiVoid) ZuiControlSetFocus(ZuiControl Control)
{
	if (Control->Focusable){
		ZuiControl OldControl = Control->Window->FocusControl;
		Control->Window->FocusControl = Control;
		/*向旧控件发送失去焦点的消息*/
		if (OldControl)
		{
			ZuiDefaultControlProc(OldControl, ZM_KILLFOCUS, 0, (ZuiInt)Control);
		}
		/*向新控件发送得到焦点的消息*/
		ZuiDefaultControlProc(Control, ZM_SETFOCUS, 0, (ZuiInt)OldControl);
	}
}

ZAPI(ZuiBool) ZuiControlGetFocus(ZuiControl Control)
{
	return Control->Window->FocusControl == Control;
}

ZAPI(ZuiVoid) ZuiControlSetCapture(ZuiControl Control){
	Control->Window->CaptureControl = Control;
	ZuiOsSetCapture(Control->Window);
}

ZAPI(ZuiVoid) ZuiControlReleaseCapture(ZuiControl Control){
	Control->Window->CaptureControl = NULL;
	ZuiOsReleaseCapture();
}

ZAPI(ZuiVoid) ZuiControlSetDefaultEventProc(ZCtlEventProc EventProc) {
	DefEventProc = EventProc;
}

ZAPI(ZuiVoid) ZuiControlSetCtlProc(ZuiControl Control, ZCtlProc CtlProc) {
	Control->CtlProc = CtlProc;
}

ZAPI(ZCtlProc) ZuiControlGetCtlProc(ZuiControl Control) {
	return Control->CtlProc;
}

ZAPI(ZuiVoid) ZuiControlSetEventProc(ZuiControl Control, ZCtlEventProc EventProc) {
	Control->EventProc = EventProc;
}

ZAPI(ZCtlEventProc) ZuiControlGetEventProc(ZuiControl Control) {
	return Control->EventProc;
}

ZAPI(ZuiInt) ZuiControlTriggerEvent(ZuiControl Control, ZuiInt Event, ZuiInt Param1, ZuiInt Param2, ZuiInt Param3) {
	if (Control == NULL) {
		return 0;
	}
	if (Control->EventProc != NULL) {
		return Control->EventProc(Control->Window, Control, Event, Param1, Param2, Param3);
	}
	else if (DefEventProc != NULL) {
		return DefEventProc(Control->Window, Control, Event, Param1, Param2, Param3);
	}
	return 0;
}

ZAPI(ZuiInt) ZuiControlSendMessage(ZuiControl Control, ZuiInt Message, ZuiInt Param1, ZuiInt Param2) {
	if (Control == NULL) {
		return 0;
	}
	return ZuiDefaultControlProc(Control, Message, Param1, Param2);
}

ZAPI(ZuiVoid) ZuiControlSendMessageToChild(ZuiControl ParentControl, ZuiInt Message, ZuiInt Param1, ZuiInt Param2) {
	ZuiControl Control = ParentControl->ChildControlEnd;
	while (Control != NULL) {
		ZuiControlSendMessage(Control, Message, Param1, Param2);
		ZuiControlSendMessageToChild(Control, Message, Param1, Param2);
		Control = Control->LastControl;
	}
}

ZAPI(ZuiVoid) ZuiControlSendMessageToChildOfWindow(ZuiWindow Window, ZuiInt Message, ZuiInt Param1, ZuiInt Param2) {
	ZuiControl Control = ZuiGetWindowRootControl(Window)->ChildControlEnd;
	while (Control != NULL) {
		ZuiControlSendMessage(Control, Message, Param1, Param2);
		ZuiControlSendMessageToChild(Control, Message, Param1, Param2);
		Control = Control->LastControl;
	}
}

ZAPI(ZuiVoid) ZuiControlMove(ZuiControl Control, ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height) {
	/*是否移动或改变大小*/
	ZuiBool Moved = FALSE, Sized = FALSE;
	if (Control->Rect.Left != Left || Control->Rect.Top != Top) {
		Moved = TRUE;
	}
	if (Control->Rect.Width != Width || Control->Rect.Height != Height) {
		Sized = TRUE;
	}
	if (Width < 0 || Height < 0){
		return;
	}
	/*以前的控件矩形*/
	ZRect OldRect = Control->Rect;
	ZRect OldRectOfWindow;
	ZuiControlGetRectOfWindow(Control, &OldRectOfWindow);
	/*更新窗口矩形*/
	MAKEVRECT(Control->Rect, Left, Top, Width, Height);
	MAKEVRECT(Control->CRect, 0, 0, Width - 1, Height - 1);
	if (Moved) {
		/*通知控件位置被移动*/
		ZuiControlSendMessage(Control, ZM_MOVE, 0, (ZuiInt)&OldRect.Left);
	}
	if (Sized) {
		/*大小被更改*/
		if (!Control->Virtual) {
			/*销毁掉控件当前的图形*/
			ZuiDestroyGraphics(Control->Graphics);
			/*重新为控件创建图形*/
			Control->Graphics = ZuiCreateGraphicsInMemory(Width, Height);
			/*通知控件重绘*/
			ZuiControlSendMessage(Control, ZM_REDRAW, 0, 0);
			ZuiWindowUpdateGraphicsRect(Control->Window, &OldRectOfWindow, FALSE, FALSE);
			ZRect NewRectOfWindow;
			ZuiControlGetRectOfWindow(Control, &OldRectOfWindow);
			ZuiWindowUpdateGraphicsRect(Control->Window, &NewRectOfWindow, FALSE, FALSE);
		}
		/*通知控件大小被改变*/
		ZuiControlSendMessage(Control, ZM_SIZE, 0, (ZuiInt)OldRect.Width);
	}
}

ZAPI(ZuiRect) ZuiControlGetRect(ZuiControl Control) {
	return &Control->Rect;
}

ZAPI(ZuiRect) ZuiControlGetRectOfWindow(ZuiControl Control, ZuiRect Rect) {
	*Rect = Control->Rect;
	ZuiControl ParentControl = Control->ParentControl;
	while (ParentControl) {
		Rect->Left += ParentControl->Rect.Left;
		Rect->Top += ParentControl->Rect.Top;
		ParentControl = ParentControl->ParentControl;
	}
	return Rect;
}

ZAPI(ZuiControl) ZuiFindControlInWindow(ZuiWindow Window, ZuiInt x, ZuiInt y, ZuiInt *x1, ZuiInt *y1) {
	ZuiControl Control = ZuiFindControlInControl(ZuiGetWindowRootControl(Window), x, y, x1, y1);
	if (!Control) {
		*x1 = x;
		*y1 = y;
		return ZuiGetWindowRootControl(Window);
	}
	return Control;
}

ZAPI(ZuiControl) ZuiFindControlInControl(ZuiControl ParentControl, ZuiInt x, ZuiInt y, ZuiInt *x1, ZuiInt *y1) {
	/*从最后一个子控件开始寻找 倒序*/
	ZuiControl Control = ParentControl->ChildControlEnd;
	while (Control != NULL) {
		if (!Control->Visible || !ZuiControlIsEnable(Control) || Control->MousePenetration) {
			/*不满足可被鼠标命中的条件 处理上一个子控件*/
			Control = Control->LastControl;
			continue;
		}
		if (ZuiIsPointInRect(x, y, &Control->Rect)){
			/*当前处理的控件被命中*/
			/*计算客户区坐标*/
			*x1 = x - Control->Rect.Left;
			*y1 = y - Control->Rect.Top;
			/*递归子控件*/
			ZuiControl SubControl = ZuiFindControlInControl(Control, x, y, x1, y1);
			if (SubControl == NULL) {
				/*子控件被未被命中 返回当前控件*/
				return Control;
			}
			else {
				/*子控件被命中 返回子控件*/
				return SubControl;
			}
		}
		/*处理上一个子控件*/
		Control = Control->LastControl;
	}
	return NULL;
}
/**
*通过ZuiControlSendMessage调用过来
*/
ZuiInt ZuiDefaultControlProc(ZuiControl Control, ZuiInt Message, ZuiInt Param1, ZuiInt Param2) {
	/*处理内核事件*/
	switch (Message) {
	case ZM_SIZE: {
		/*大小被改变*/
		if (Control->Virtual) {
			return 0;
		}
		ZuiDestroyGraphics(Control->Graphics_Gradient1);
		ZuiDestroyGraphics(Control->Graphics_Gradient2);
		ZuiDestroyGraphics(Control->Graphics);
		Control->Graphics = ZuiCreateGraphicsInMemory(Control->Rect.Width, Control->Rect.Height);
		ZuiControlSendMessage(Control, ZM_REDRAW, 0, 0);
		break;
	}

	case ZM_UPDATE: {
		if (Control->Virtual) {
			return 0;
		}
		ZRect RectOfWindow;
		ZuiControlGetRectOfWindow(Control, &RectOfWindow);
		ZuiWindowUpdateGraphicsRect(Control->Window, &RectOfWindow, FALSE, TRUE);
		break;
	}

	case ZM_REDRAW: {
		if (Control->Virtual) {
			return 0;
		}
		ZuiGraphicsClear(Control->Graphics, 0);
		ZuiInt Result = Control->CtlProc(Control, ZM_PAINT, 0, (ZuiInt)Control->Graphics);
#if (defined DEBUG_BORDER) && (DEBUG_BORDER == 1)
		ZuiDrawRect(Control->Graphics, ARGB(255, 0, 0, 0), 0, 0, Control->Rect.Width - 1, Control->Rect.Height - 1, 1);
#endif
		if (Param1) {
			ZuiControlSendMessage(Control, ZM_UPDATE, 0, 0);
		}
		return Result;
		break;
	}
	case ZM_TIMER:{
		if (Param2 == Control->GradientTimer)
		{
			/*渐变时钟*/
			Control->GradientAlpha += (ZuiReal)(Param1 * 2);//计算本次的透明度
			if (Control->GradientAlpha > 255){ Control->GradientAlpha = 255; }//透明度最多255
			if (Control->GradientType){
				//在旧的缓存图形上慢慢增加不透明度覆盖绘制新的图像(模拟SyserUI工作方式)
				ZuiAlphaBlend(Control->Graphics, 0, 0, Control->CRect.Width, Control->CRect.Height, Control->Graphics_Gradient2, 0, 0, 255);
			}
			else
			{
				//慢慢增加新图像的不透明度,减少就图像的不透明度(模拟Ex_DirectUI2.0工作方式)
				ZuiGraphicsClear(Control->Graphics, 0);
				ZuiAlphaBlend(Control->Graphics, 0, 0, Control->CRect.Width, Control->CRect.Height, Control->Graphics_Gradient2, 0, 0, 255 - (ZuiByte)Control->GradientAlpha);
			}
			ZuiAlphaBlend(Control->Graphics, 0, 0, Control->CRect.Width, Control->CRect.Height, Control->Graphics_Gradient1, 0, 0, (ZuiByte)Control->GradientAlpha);
			/*刷新缓存*/
			/*刷新显示*/
			ZuiDefaultControlProc(Control, ZM_UPDATE, 0, 0);
			if (Control->GradientAlpha == 255)
			{
				ZuiAlphaBlend(Control->Graphics, 0, 0, Control->CRect.Width, Control->CRect.Height, Control->Graphics_Gradient1, 0, 0, Control->Alpha);
				ZuiDefaultControlProc(Control, ZM_UPDATE, 0, 0);
				/*销毁时钟*/
				ZuiOsDestroyTimer(Control->GradientTimer);
				Control->GradientTimer = 0;
				Control->GradientAlpha = 0;
				Control->Gradienting = FALSE;
			}
		}
		break;
	}
	case ZM_SETENABLED: {
		if (Param2 != 0) {
			Control->DisabledCount = Control->DisabledCount - Param2;
			return 0;
		}
		break;
	}

	case ZM_SETVISIBLE: {
		if (Param2 != 0) {
			Control->InvisibleCount = Control->InvisibleCount - Param2;
			return 0;
		}
		break;
	}
	}
	/*处理内核事件结束*/
	/*通知控件处理*/
	ZuiInt Result = 0;
	if (Control->CtlProc != NULL) {
		Result = Control->CtlProc(Control, Message, Param1, Param2);
	}
	/*控件处理结束*/
	/*通知用户处理*/
	switch (Message) {
	case ZM_LBUTTONDOWN: {
		ZuiControlTriggerEvent(Control, ZE_LBTNDOWN, Param2, 0, 0);
		break;
	}

	case ZM_MBUTTONDOWN: {
		ZuiControlTriggerEvent(Control, ZE_MBTNDOWN, Param2, 0, 0);
		break;
	}

	case ZM_RBUTTONDOWN: {
		ZuiControlTriggerEvent(Control, ZE_RBTNDOWN, Param2, 0, 0);
		break;
	}

	case ZM_LBUTTONUP: {
		ZuiControlTriggerEvent(Control, ZE_LBTNUP, Param2, 0, 0);
		break;
	}

	case ZM_MBUTTONUP: {
		ZuiControlTriggerEvent(Control, ZE_MBTNUP, Param2, 0, 0);
		break;
	}

	case ZM_RBUTTONUP: {
		ZuiControlTriggerEvent(Control, ZE_RBTNUP, Param2, 0, 0);
		break;
	}

	case ZM_LBUTTONDBCLK: {
		ZuiControlTriggerEvent(Control, ZE_LBTNDBCLK, Param2, 0, 0);
		break;
	}

	case ZM_MBUTTONDBCLK: {
		ZuiControlTriggerEvent(Control, ZE_MBTNDBCLK, Param2, 0, 0);
		break;
	}

	case ZM_RBUTTONDBCLK: {
		ZuiControlTriggerEvent(Control, ZE_RBTNDBCLK, Param2, 0, 0);
		break;
	}

	case ZM_LBUTTONCLK: {
		ZuiControlTriggerEvent(Control, ZE_LBTNCLK, Param2, 0, 0);
		break;
	}

	case ZM_MBUTTONCLK: {
		ZuiControlTriggerEvent(Control, ZE_MBTNCLK, Param2, 0, 0);
		break;
	}

	case ZM_RBUTTONCLK: {
		ZuiControlTriggerEvent(Control, ZE_RBTNCLK, Param2, 0, 0);
		break;
	}
	}
	/*用户处理结束*/
	return Result;
}
