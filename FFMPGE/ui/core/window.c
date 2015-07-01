#include "window.h"
ZAPI(ZuiWindow) ZuiCreateWindow(ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height,
	ZuiInt WindowStyle,
	ZuiText Title,
	ZuiStringFormat StringFormat,
	ZuiImage BackgroundImage,
	ZuiColor BackgroundColor,
	ZuiInt BackgroundType,
	ZuiInt Shape
	) {
	ZuiWindow Window = (ZuiWindow)malloc(sizeof(ZWindow));
	memset(Window, 0, sizeof(ZWindow));

	ZuiOsWindow OsWindow = ZuiOsCreateWindow(Left, Top, Width, Height, Title, !(WindowStyle & VWS_NOTASKBAR), (WindowStyle & VWS_POSMIDDLE), Window);
	if (NULL == OsWindow) {
		free(Window);
		return NULL;
	}
	Window->OsWindow = OsWindow;	//保存移植层的window对象
	

	Window->BackgroundColor = BackgroundColor;
	Window->BackgroundImage = BackgroundImage;
	Window->BackgroundType = BackgroundType;

	Window->DragType = WindowStyle & (VWS_DRAG_ANY | VWS_DRAG_NO | VWS_DRAG_TITLE);//任意拖动

	Window->ShadowColor = -1;//阴影颜色 默认无

	Window->Title_StringFormat = StringFormat;
	Window->Title_Title = Title;
	ZuiSetWindowShape(Window, Shape);//窗口形状

//	Window->TaskQueue = new VTaskQueue(Window);//创建任务列队

	/*创建根控件*/
	Window->RootControl.Control = ZuiControlCreate((ZuiControl)(-(ZuiInt)Window), 0, 0, Window->Rect.Width, Window->Rect.Height, &Window->RootControl, TRUE, TRUE, NULL);
	Window->RootControl.Window = Window;
	Window->RootControl.Control->CtlProc = ZuiWindowDefaultControlsProc;
	Window->RootControl.ID = VWDC_ROOT;
	if (WindowStyle & VWS_TITLE) {
		/*创建标题栏 作为跟控件的子控件*/
		Window->Title.Control = ZuiControlCreate(ZuiGetWindowRootControl(Window), 0, 0, Window->Rect.Width, 28, &Window->Title, TRUE, TRUE, NULL);
		Window->Title.Window = Window;
		Window->Title.Control->CtlProc = ZuiWindowDefaultControlsProc;
		Window->Title.ID = VWDC_TITLE;
	}
	/*创建窗口相关Graphics对象*/
	ZuiWindowInitGraphics(Window, TRUE);
	return Window;
}

ZAPI(ZuiVoid) ZuiDestroyWindow(ZuiWindow Window) {
	/*销毁窗口根控件 与其相关的子控件都会*/
	ZuiControlDestroy(Window->RootControl.Control);
	/*销毁背景图形*/
	ZuiDestroyGraphics(Window->GraphicsBackground);
	/*销毁窗口图形*/
	ZuiDestroyGraphics(Window->GraphicsWindow);
	/*释放内存*/
	free(Window);
}

ZAPI(ZuiVoid) ZuiSetWindowEventProc(ZuiWindow Window, ZCtlEventProc EventProc) {
	Window->RootControl.Control->EventProc = EventProc;
}

ZAPI(ZCtlEventProc) ZuiGetWindowEventProc(ZuiWindow Window) {
	return Window->RootControl.Control->EventProc;
}

ZAPI(ZuiVoid) ZuiSetWindowVisible(ZuiWindow Window, ZuiBool Visible) {
	if (Visible && Window->FirstShow == FALSE) {
		Window->FirstShow = TRUE;
	}
	ZuiOsSetWindowVisible(Window->OsWindow, Visible);
}

ZAPI(ZuiBool) ZuiGetWindowVisible(ZuiWindow Window) {
	return ZuiOsGetWindowVisible(Window->OsWindow);
}

ZAPI(ZuiVoid) ZuiSetSizeTunable(ZuiWindow Window, ZuiBool Tunable){
	if (Tunable && Window->SizeTunable == FALSE) {
		Window->SizeTunable = TRUE;
	}
}

ZAPI(ZuiBool) ZuiGetSizeTunable(ZuiWindow Window, ZuiBool Tunable){
	return Window->SizeTunable;
}

ZAPI(ZuiControl) ZuiGetWindowRootControl(ZuiWindow Window) {
	return Window->RootControl.Control;
}

ZAPI(ZuiVoid) ZuiSetWindowBkg(ZuiWindow Window, ZuiImage BackgroundImage, ZuiColor BackgroundColor, ZuiInt BackgroundType) {
	Window->BackgroundColor = BackgroundColor;
	Window->BackgroundImage = BackgroundImage;
	Window->BackgroundType = BackgroundType;
	ZuiWindowInitGraphics(Window, FALSE);
}

ZAPI(ZuiVoid) ZuiSetWindowShape(ZuiWindow Window, ZuiInt Shape) {
	Window->Shape = Shape;
}

ZAPI(ZuiInt) ZuiGetWindowShape(ZuiWindow Window) {
	return Window->Shape;
}

ZAPI(ZuiVoid) ZuiSetWindowShadowColor(ZuiWindow Window, ZuiInt ShadowColor) {
	Window->ShadowColor = ShadowColor;
}

ZAPI(ZuiInt) ZuiGetWindowShadowColor(ZuiWindow Window) {
	return Window->ShadowColor;
}

ZAPI(ZuiVoid) ZuiRedrawWindowGraphics(ZuiWindow Window) {
	ZuiWindowInitGraphics(Window, FALSE);
}

ZAPI(ZuiVoid) ZuiRedrawWindow(ZuiWindow Window) {
	ZuiRedrawWindowGraphics(Window);
	ZuiFlashWindow(Window);
}

ZAPI(ZuiVoid) ZuiFlashWindow(ZuiWindow Window) {
	ZuiWindowUpdate(Window, NULL);
}

ZAPI(ZuiVoid) ZuiSetWindowComposite(ZuiWindow Window, ZuiBool Composite) {
	ZuiOsSetWindowComposite(Window->OsWindow, Composite);
}

ZAPI(ZuiBool) ZuiGetWindowComposite(ZuiWindow Window) {
	return ZuiOsGetWindowComposite(Window->OsWindow);
}

ZAPI(ZuiVoid) ZuiSetWindowAlpha(ZuiWindow Window, ZuiByte Alpha) {
	Window->Alpha = Alpha;
}

ZAPI(ZuiByte) ZuiGetWindowAlpha(ZuiWindow Window) {
	return Window->Alpha;
}

ZuiVoid ZuiWindowInitGraphics(ZuiWindow Window, ZuiBool ForceRecreate) {
	if (ForceRecreate) {
		/*强制重建*/
		ZuiDestroyGraphics(Window->GraphicsBackground);
		ZuiDestroyGraphics(Window->GraphicsWindow);
		Window->GraphicsBackground = ZuiCreateGraphicsInMemory(Window->Rect.Width, Window->Rect.Height);
		Window->GraphicsWindow = ZuiCreateGraphicsInMemory(Window->Rect.Width, Window->Rect.Height);
	}
	else {
		ZuiGraphicsClear(Window->GraphicsBackground, 0);
		ZuiGraphicsClear(Window->GraphicsWindow, 0);
	}
	/*重画背景*/
	ZuiWindowDrawBackground(Window, Window->GraphicsBackground);
	/*重画窗口*/
	ZuiWindowDrawWindow(Window, Window->GraphicsWindow);
}

ZuiVoid ZuiWindowDrawWindow(ZuiWindow Window, ZuiGraphics Graphics) {
	//ZuiWindowDrawBackground(Window, Graphics);
	ZuiWindowDrawControl(Window);
}

ZuiVoid ZuiWindowDrawBackground(ZuiWindow Window, ZuiGraphics Graphics) {
	ZuiWindowDrawBackgroundImage(Window, Graphics);

	//HICON Icon = (HICON)SendMessageW (Window->OsWindow->hWnd, WM_GETICON, ICON_SMALL, NULL);
	//if (0 != Icon) {
	//	ZuiDrawIcon(Graphics, Icon, 7 + 8, 6 + 8, 16, 16);
	//	Window->TitleRect->Top = 3 + 8;
	//	Window->TitleRect->Left = 28 + 8;
	//} else {
	//	Window->TitleRect.Top = 3 + 8;
	//	Window->TitleRect.Left = 5 + 8;
	//}
	//ZuiDrawString(Graphics, Window->TitleStringFormat, STR(Window->Title), &Window->TitleRect);
}

ZuiVoid ZuiWindowDrawBackgroundImage(ZuiWindow Window, ZuiGraphics Graphics) {
	if (!(Window->BackgroundType & VBT_NOCOLORFILL)) {
		ZuiFillRect(Graphics, Window->BackgroundColor, 0, 0, Window->Rect.Width, Window->Rect.Height);
	}
	int ShadowColor = Window->ShadowColor;

	//if(Window->BackgroundImage) ZuiDrawImageEx(Window->GraphicsBackground, Window->BackgroundImage, 0, 0, 0, 0, 0, 0, 0, 0, 255);
	//DEBUG_PUTPNG(Window->GraphicsBackground->Bitmap, "CREATE.png");

	if (Window->BackgroundImage) {
		/*绘制背景图片*/
		if (Window->BackgroundType & VBT_LEFTTOP) {
			ZuiDrawImage(Graphics, Window->BackgroundImage, (ShadowColor == -1) ? 0 : 5, (ShadowColor == -1) ? 0 : 5);
		}
		else {
			ZuiDrawImageEx(Window->GraphicsBackground, Window->BackgroundImage, (ShadowColor == -1) ? 0 : 5, (ShadowColor == -1) ? 0 : 5, Window->Rect.Width - ((ShadowColor == -1) ? 0 : 10), Window->Rect.Height - ((ShadowColor == -1) ? 0 : 10), 0, 0, 0, 0, 255);
			//ZuiDrawImageEx(Graphics, Window->BackgroundImage, 5, 5, Window->Rect.Width - 10, Window->Rect.Height - 10, 0, 0, 0, 0, 255);
		}
	}
	if (Window->Shape == VWFS_RECT) {
		if (ShadowColor != -1) {
			/*双层边框*/
			ZuiDrawRect(Graphics, ARGB(204, 96, 96, 96), 5, 5, Window->Rect.Width - 10, Window->Rect.Height - 10, 1);
			ZuiDrawRect(Graphics, ARGB(102, 255, 255, 255), 6, 6, Window->Rect.Width - 12, Window->Rect.Height - 12, 1);

			/*绘制阴影*/
			ZuiDrawRoundRect(Graphics, RGB2ARGB(ShadowColor, 45), 5, 5, Window->Rect.Width - 10, Window->Rect.Height - 10, 1, 0);
			ZuiDrawRoundRect(Graphics, RGB2ARGB(ShadowColor, 25), 4, 4, Window->Rect.Width - 8, Window->Rect.Height - 8, 1, 0);
			ZuiDrawRoundRect(Graphics, RGB2ARGB(ShadowColor, 10), 3, 3, Window->Rect.Width - 6, Window->Rect.Height - 6, 1, 0);
			ZuiDrawRoundRect(Graphics, RGB2ARGB(ShadowColor, 5), 2, 2, Window->Rect.Width - 4, Window->Rect.Height - 4, 1, 0);
		}
		else{
			/*双层边框*/
			ZuiDrawRect(Graphics, ARGB(204, 96, 96, 96), 0, 0, Window->Rect.Width - 1, Window->Rect.Height - 1, 1);
			ZuiDrawRect(Graphics, ARGB(102, 255, 255, 255), 1, 1, Window->Rect.Width - 3, Window->Rect.Height - 3, 1);
		}
	}
}

ZuiVoid ZuiWindowDrawControl(ZuiWindow Window) {
	ZuiWindowUpdateGraphicsRect(Window, NULL, TRUE, FALSE);
}

ZuiVoid ZuiWindowUpdate(ZuiWindow Window, ZuiRect UpdateRect) {
	ZuiOsUpdateWindow(Window, UpdateRect);
}

ZuiVoid ZuiWindowUpdateGraphicsRect(ZuiWindow Window, ZuiRect UpdateRect, ZuiBool ForceRedraw, ZuiBool Flash) {
	if (!UpdateRect) {
		ZRect r;
		/*更新矩形为空的话则更新整个窗口*/
		UpdateRect = &r;
		MAKEVRECTP(UpdateRect, 0, 0, Window->Rect.Width, Window->Rect.Height);
	}
	/*复制更新区域的背景到窗口*/
	ZuiAlphaBlend(Window->GraphicsWindow, UpdateRect->Left, UpdateRect->Top, UpdateRect->Width, UpdateRect->Height, Window->GraphicsBackground, UpdateRect->Left, UpdateRect->Top, 255);

	ZuiControl Control = Window->RootControl.Control->ChildControlFirst;
	ZPoint OffsetPoint;
	MAKEVPOINT(OffsetPoint, 0, 0);
	ZuiByte Alpha = 255;
	while (Control) {
		ZRect Intersect;
		if (!ZuiControlIsVisible(Control)) {
			/*控件不可视 更新下一个同级控件*/
			Control = Control->NextControl;
			/*到循环尾*/
			continue;
		}
		ZRect RectOfClient;
		MAKEVRECT(RectOfClient, Control->Rect.Left + OffsetPoint.x, Control->Rect.Top + OffsetPoint.y, Control->Rect.Width, Control->Rect.Height);
		if (!ZuiIntersectRect(&RectOfClient, UpdateRect, &Intersect)) {
			/*控件不在更新范围内 更新下一个同级控件*/
			Control = Control->NextControl;
			/*到循环尾*/
			continue;
		}
		if (!Control->Virtual) {
			if (ForceRedraw) {
				ZuiControlSendMessage(Control, ZM_REDRAW, 0, 0);
			}
			ZuiAlphaBlend(Window->GraphicsWindow, Intersect.Left, Intersect.Top, Intersect.Width, Intersect.Height, Control->Graphics, 0, 0, Alpha * Control->Alpha / 255);
		}

		if (Control->ChildControlFirst != NULL) {
			OffsetPoint.x = OffsetPoint.x + Control->Rect.Left;
			OffsetPoint.y = OffsetPoint.y + Control->Rect.Top;
			Control = Control->ChildControlFirst;
			Alpha = Alpha * Control->Alpha / 255;
		}
		else if (Control->NextControl != NULL) {
			/*更新下一个同级控件*/
			Control = Control->NextControl;
		}
		else {
			/*已经最后一个控件了*/
			while (Control->NextControl == NULL) {
				if (Control->ParentControl == NULL) {
					if (Flash) {
						ZuiWindowUpdate(Window, UpdateRect);
					}
					return;
				}
				OffsetPoint.x = OffsetPoint.x - Control->ParentControl->Rect.Left;
				OffsetPoint.y = OffsetPoint.y - Control->ParentControl->Rect.Top;
				Alpha = Alpha / Control->Alpha * 255;
				Control = Control->ParentControl;
			}
			/**/
			Control = Control->NextControl;
		}
	}
	if (Flash) {
		ZuiWindowUpdate(Window, UpdateRect);
	}
}
/**
* window类的消息回调
*/
ZuiInt ZuiWindowDefaultControlsProc(ZuiControl Control ,ZuiInt Message, ZuiInt Param1, ZuiInt Param2) {
	ZuiWindowDefaultControl ControlInfo = (ZuiWindowDefaultControl)Control->ControlData;
	if (ControlInfo->ID == VWDC_ROOT) {
		ZuiWindow Window = ControlInfo->Window;
		/*窗口消息*/
		switch (Message) {
		case ZM_LBUTTONDOWN:
			if (ControlInfo->Window->DragType & VWS_DRAG_ANY) {
				/*任意移动窗口*/
				ZuiOsDragWindow(ControlInfo->Window->OsWindow);
			}
			break;
		case ZM_SIZE:
			{
				Window->Rect.Width = Param1;
				Window->Rect.Height = Param2;
				ZuiControlMove(Window->Title.Control, Window->Title.Control->Rect.Left, Window->Title.Control->Rect.Top, Window->Rect.Width - Window->Title.Control->Rect.Left, Window->Title.Control->Rect.Height);
				if (Window->FirstShow)
				{
					ZuiWindowInitGraphics(Window, TRUE);
					ZuiOsUpdateWindow(Window, NULL);
				}
				ZuiControlTriggerEvent(Control, ZM_SIZE, Param1, Param2, NULL);//通知用户大小被改变
			}
			break;
		case ZM_MOVE:{
			/*更新ZuiWindow对象窗口位置*/
				Window->Rect.Left = Param1;
				Window->Rect.Top = Param2;
				ZuiControlTriggerEvent(Control, ZM_SIZE, Param1, Param2, NULL);//通知用户位置被移动
			}
			break;
		default:
			break;
		}
		return 0;
	}
	else if (ControlInfo->ID == VWDC_TITLE) {
		/*标题区消息*/
		switch (Message) {
		case ZM_LBUTTONDOWN:{
			if (!(ControlInfo->Window->DragType & VWS_DRAG_NO)) {
				/*任意移动窗口*/
				ZuiOsDragWindow(ControlInfo->Window->OsWindow);
			}
			break;
		}
		case ZM_PAINT:{
			ZuiDrawString((ZuiGraphics)Param2, ControlInfo->Window->Title_StringFormat, ControlInfo->Window->Title_Title, &ControlInfo->Control->Rect);
			/*重绘事件*/
			break;
		}
		}
		//return Control->CtlProc(Control, Message, Param1, Param2);
	}
	return 0;
}
/*---------XML绑定------------*/
ZuiXmlBuilder_Window()
{

}
/*---------LUA绑定------------*/
ZuiLuaBuilder_Window()
{

}
