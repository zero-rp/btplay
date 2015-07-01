
#include "win.h"
#include "../../core/window.h"

#if (defined PLATFORM_OS_WIN)


LRESULT CALLBACK ZuiOsWin32WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

ZuiBool ZuiOsInitialize() {
	//hInstance = GetModuleHandleA(NULL);
	return TRUE;
}

/*生成时钟回调*/
void *ZuiOsMarkTimerProc(ZuiControl Control, ZuiInt Param1)
{
	/*
	push ebp				;
	mov ebp,esp				;
	pushf					;保存标志位
	push 0x1000000			;时钟句柄
	push 0x1000000			;参数1
	push 10					;VM_TIMER
	push 0x1000000			;控件指针
	mov eax,111				;VanillaDefaultControlProc
	call eax				;通知控件
	add esp,0x10			;平栈
	popf					;恢复标志位
	mov esp,ebp				;
	pop ebp					;
	ret 0x10				;返回
	0x0						;时钟句柄
	*/
	unsigned char b[38] = { 85, 137, 229, 156, 104, 0, 0, 0, 1, 104, 0, 0, 0, 1, 106, 10, 104, 0, 0, 0, 1, 184, 111, 0, 0, 0, 255, 208, 131, 196, 16, 157, 137, 236, 93, 194, 16, 0 };
	void *f = malloc(42);
	int p = (int)&ZuiDefaultControlProc;
	*(int *)(b + 5) = f;
	*(int *)(b + 10) = Param1;
	*(int *)(b + 17) = Control;
	*(int *)(b + 22) = p;
	memcpy(f, b, 38);
	return f;
}
ZuiTimer ZuiOsCreateTimer(ZuiInt nElapse, ZuiControl Control, ZuiInt Param1){
	void *p = ZuiOsMarkTimerProc(Control, Param1);
	HWND t = SetTimer(0, 0, nElapse, (TIMERPROC)p);
	*(int *)((unsigned char *)p + 38) = t;
	return p;
}
ZuiVoid ZuiOsDestroyTimer(ZuiTimer Timer)
{
	KillTimer(0,*(int *)(Timer+38));
	free(Timer);
}
ZuiOsWindow ZuiOsCreateWindow(ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height, ZuiText Title, ZuiBool ShowInTaskbar, ZuiBool PosMiddle, ZuiWindow Window) {
	static ZuiBool ClassRegistered;//是否注册WNDCLASS
	if (!ClassRegistered) {
		/*第一次调用该函数向系统注册ZuiUI.Window类*/
		WNDCLASSEXW WindowClass;
		memset(&WindowClass, 0, sizeof(WindowClass));
		WindowClass.cbSize = sizeof(WindowClass);
		WindowClass.lpfnWndProc = (WNDPROC)&ZuiOsWin32WindowProc;
		WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		WindowClass.lpszClassName = L"ZuiUI.Window";
		WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);/*箭头光标*/
		WindowClass.hInstance = GetModuleHandleA(NULL);
		RegisterClassExW(&WindowClass);
		ClassRegistered = 1;
	}
	if (PosMiddle) {
		/*窗口居中*/
		Left = (GetSystemMetrics(SM_CXSCREEN) - Width) / 2;
		Top = (GetSystemMetrics(SM_CYSCREEN) - Height) / 2;
	}
	MAKEVRECT(Window->Rect, Left, Top, Width, Height);
	HWND hWnd = CreateWindowExW(ShowInTaskbar ? FALSE : WS_EX_TOOLWINDOW,
		L"ZuiUI.Window",
		Title,
		-1811937280,
		Left,
		Top,
		Width,
		Height,
		NULL,
		NULL,
		NULL,
		NULL
		);
	if (!IsWindow(hWnd)) {
		return NULL;
	}
	/*保存相关参数到VOsWindow*/
	ZuiOsWindow OsWindow = (ZuiOsWindow)malloc(sizeof(ZOsWindow));
	if (OsWindow)
	{
		memset(OsWindow, 0, sizeof(ZOsWindow));
		OsWindow->hWnd = hWnd;
		OsWindow->hDC = GetDC(hWnd);
		OsWindow->hIMC = ImmGetContext(hWnd);//获取系统的输入法
		/*屏蔽输入法*/
		ImmAssociateContext(hWnd, NULL);
		SetPropW(OsWindow->hWnd, PROP_ID, (HANDLE)Window);
		return OsWindow;
	}
	return NULL;
}
ZuiVoid ZuiOsDestroyWindow(ZuiOsWindow OsWindow) {
	if (OsWindow) {
		/*释放由ImmGetContext得到的输入法句柄*/
		ImmReleaseContext(OsWindow->hWnd, OsWindow->hIMC);
		/*释放由GetDc得到的窗口DC*/
		ReleaseDC(OsWindow->hWnd, OsWindow->hDC);
		/*释放窗口*/
		DestroyWindow(OsWindow->hWnd);
		free(OsWindow);
	}
}
ZuiVoid ZuiOsSetWindowTitle(ZuiOsWindow OsWindow, ZuiText Title) {
	if (OsWindow) {
		SetWindowTextW(OsWindow->hWnd, Title);
	}
}
ZuiInt ZuiOsGetWindowTitle(ZuiOsWindow OsWindow, ZuiText out, ZuiInt outlen) {
	if (OsWindow) {
		return GetWindowTextW(OsWindow->hWnd, out, outlen);
	}
	return 0;
}
ZuiVoid ZuiOsSetWindowVisible(ZuiOsWindow OsWindow, ZuiBool Visible) {
	if (OsWindow) {
		ShowWindow(OsWindow->hWnd, Visible ? SW_SHOW : SW_HIDE);
	}
}
ZuiBool ZuiOsGetWindowVisible(ZuiOsWindow OsWindow) {
	if (OsWindow) {
		return IsWindowVisible(OsWindow->hWnd) == TRUE;
	}
	return 0;
}
ZuiVoid ZuiOsSetWindowComposite(ZuiOsWindow OsWindow, ZuiBool Composite) {
	if (OsWindow) {
		OsWindow->Layered = Composite;
		if (Composite) {
			SetWindowLongW(OsWindow->hWnd, GWL_EXSTYLE, GetWindowLongW(OsWindow->hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		}
		else {
			SetWindowLongW(OsWindow->hWnd, GWL_EXSTYLE, GetWindowLongW(OsWindow->hWnd, GWL_EXSTYLE) | ~WS_EX_LAYERED);;
		}
	}
}
ZuiBool ZuiOsGetWindowComposite(ZuiOsWindow OsWindow) {
	if (OsWindow) {
		return OsWindow->Layered;
	}
	return 0;
}
ZuiVoid ZuiOsDragWindow(ZuiOsWindow OsWindow) {
	if (OsWindow) {
		SendMessageW(OsWindow->hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	return;
}
ZuiVoid ZuiOsUpdateWindow(ZuiWindow Window, ZuiRect UpdateRect) {
	if (Window) {
		if (Window->OsWindow->Layered) {
			/*分层窗口*/
			BLENDFUNCTION Blend;
			Blend.AlphaFormat = AC_SRC_ALPHA;
			Blend.BlendFlags = 0;
			Blend.BlendOp = 0;
			Blend.SourceConstantAlpha = Window->Alpha;

			POINT pt1 = { Window->Rect.Left, Window->Rect.Top };
			POINT pt2 = { 0, 0 };
			SIZE sz = { Window->Rect.Width, Window->Rect.Height };
			HDC SrcDC = Window->GraphicsWindow->hdc;
			UpdateLayeredWindow(Window->OsWindow->hWnd,
				Window->OsWindow->hDC,
				&pt1,
				&sz,
				SrcDC,
				&pt2,
				0,
				&Blend,
				ULW_ALPHA);
		}
		else {
			/*普通窗口*/
			ZRect _UpdateRect;
			if (NULL == UpdateRect) {
				_UpdateRect = Window->Rect;
				_UpdateRect.Left = 0;
				_UpdateRect.Top = 0;
				UpdateRect = &_UpdateRect;
			}
			BitBlt(Window->OsWindow->hDC, UpdateRect->Left, UpdateRect->Top, UpdateRect->Width, UpdateRect->Height, Window->GraphicsWindow->hdc, UpdateRect->Left, UpdateRect->Top, SRCCOPY);
		}
	}
}
ZuiVoid ZuiOsSetCursor(ZuiWindow Window, ZuiInt Cursor)
{
#ifdef _WIN64
	SetClassLong(Window->OsWindow->hWnd, GCLP_HCURSOR, (LONG)LoadCursor(NULL, (LPCWSTR)Cursor));
#else
	SetClassLong(Window->OsWindow->hWnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, (LPCWSTR)Cursor));
#endif
}
ZuiVoid ZuiOsSetCapture(ZuiWindow Window){
	SetCapture(Window->OsWindow->hWnd);
}
ZuiVoid ZuiOsReleaseCapture(){
	ReleaseCapture();
}
ZuiVoid ZuiOsSetImeStatus(ZuiWindow Window, ZuiBool Status){
	if (Status){//打开
		ImmAssociateContext(Window->OsWindow->hWnd, Window->OsWindow->hIMC);
	}
	else{//关闭
		ImmAssociateContext(Window->OsWindow->hWnd, NULL);
	}
}
ZuiVoid ZuiOsSetImeCurrentPos(ZuiWindow Window, ZuiInt x, ZuiInt y){
	COMPOSITIONFORM COMPOSITIONFORM;
	COMPOSITIONFORM.dwStyle = CFS_POINT | CFS_FORCE_POSITION;
	COMPOSITIONFORM.ptCurrentPos.x = x;
	COMPOSITIONFORM.ptCurrentPos.y = y;
	ImmSetCompositionWindow(Window->OsWindow->hIMC, &COMPOSITIONFORM);
}
ZuiInt ZuiOsMessageLoop() {
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
ZuiInt ZuiOsGetTickCount() {
	return GetTickCount();
}
ZuiBool ZuiOsGetAsyncKeyState(ZuiInt vKey)
{
	return GetAsyncKeyState(vKey) && 32768;
}
ZAPI(ZuiInt) ZuiA2W(char *str, ZuiInt slen, ZuiText out, ZuiInt olen)
{
	return MultiByteToWideChar(CP_ACP, 0, str, slen, out, olen);
}

ZAPI(ZuiInt) ZuiW2A(ZuiText str, ZuiInt slen, char *out, ZuiInt olen)
{
	return WideCharToMultiByte(CP_ACP, 0, str, slen, out, olen, NULL, NULL);
}

/**
* 移植层的消息回调 来自操作系统的消息通知由此函数分发处理
*/
LRESULT CALLBACK ZuiOsWin32WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	ZuiControl Control;
	ZuiInt x1;
	ZuiInt y1;
	ZuiWindow Window = (ZuiWindow)GetPropW(hWnd, PROP_ID);//取出窗口对应的ZuiWindow对象
	if (Window == NULL) {
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}
	switch (uMsg) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			BitBlt(ps.hdc, 0, 0, Window->Rect.Width, Window->Rect.Height, Window->GraphicsWindow->hdc, 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);
			return 0;
			break;
		}
		case WM_ERASEBKGND:{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			BitBlt(ps.hdc, 0, 0, Window->Rect.Width, Window->Rect.Height, Window->GraphicsWindow->hdc, 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);
			return 0;
			break;
		}
		case WM_SYSCOMMAND:{
			if (wParam == SC_MINIMIZE){
				/*最小化*/
				Window->FirstShow = 0;
			}
			break;
		}
		case WM_SIZE: {
			/*窗口大小被改变*/
			ZuiDefaultControlProc(Window->RootControl.Control, ZM_SIZE, (ZuiInt)(short)LOWORD(lParam), (ZuiInt)(short)HIWORD(lParam));
			break;
		}
		case WM_MOVE: {
			/*窗口移动*/
			ZuiDefaultControlProc(Window->RootControl.Control, ZM_MOVE, (ZuiInt)(short)LOWORD(lParam), (ZuiInt)(short)HIWORD(lParam));
			break;
		}
		case WM_MOUSEMOVE: {
			/*鼠标移动*/
			if (Window->CaptureControl)
			{/*捕获消息*/
				ZuiDefaultControlProc(Window->CaptureControl, ZM_MOUSEMOVE, LOWORD(lParam) - Window->CaptureControl->Rect.Left, HIWORD(lParam) - Window->CaptureControl->Rect.Top);
				break;
			}
			Control = ZuiFindControlInWindow(Window, LOWORD(lParam), HIWORD(lParam), &x1, &y1);
			if (Control != Window->MouseInControl) {
				/*如果得到的控件不是鼠标移动以前所再的控件*/
				/*得到旧控件*/
				ZuiControl OldControl = Window->MouseInControl;
				/*设置新控件*/
				Window->MouseInControl = Control;
				/*向旧控件发送鼠标离开的消息*/
				if (OldControl)
				{
					ZuiDefaultControlProc(OldControl, ZM_MOUSEOUT, (ZuiInt)Control, 0);
				}
			   /*向新控件发送鼠标进入的消息*/
			   ZuiDefaultControlProc(Control, ZM_MOUSEIN, (ZuiInt)OldControl, 0);
			}
			/*向当前控件发送鼠标移动的消息*/
			ZuiDefaultControlProc(Control, ZM_MOUSEMOVE, x1, y1);
			break;
		}
		case WM_LBUTTONDOWN: {
			/*鼠标左键被按下*/
			Control = ZuiFindControlInWindow(Window, LOWORD(lParam), HIWORD(lParam), &x1, &y1);
			Window->ButtonDownControl[0] = Control;
			if (Control && Control->Focusable && Window->FocusControl != Control) {
				ZuiControl OldControl = Window->FocusControl;
				Window->FocusControl = Control;
				if (OldControl)
				{
					/*向旧控件发送失去焦点的消息*/
					ZuiDefaultControlProc(OldControl, ZM_KILLFOCUS, 0, (ZuiInt)Control);
				}
				/*向新控件发送得到焦点的消息*/
				ZuiDefaultControlProc(Control, ZM_SETFOCUS, 0, (ZuiInt)OldControl);
			}
			ZuiDefaultControlProc(Control, ZM_LBUTTONDOWN, x1, y1);
			break;
		}
		case WM_LBUTTONUP: {
			/*鼠标左键被弹起*/
			if (Window->CaptureControl)
			{/*捕获消息*/
				ZuiDefaultControlProc(Window->CaptureControl, ZM_LBUTTONUP, LOWORD(lParam) - Window->CaptureControl->Rect.Left, HIWORD(lParam) - Window->CaptureControl->Rect.Top);
				break;
			}
			Control = ZuiFindControlInWindow(Window, LOWORD(lParam), HIWORD(lParam), &x1, &y1);
			ZuiDefaultControlProc(Control,ZM_LBUTTONUP, x1, y1);
			if (Window->ButtonDownControl[0] == Control) {
				/*发送单击事件*/
				ZuiDefaultControlProc(Control, ZM_LBUTTONCLK, x1, y1);
			}
			Window->ButtonDownControl[0] = NULL;
			break;
		}
		case WM_LBUTTONDBLCLK: {
			/*鼠标左键被双击*/
			Control = ZuiFindControlInWindow(Window, LOWORD(lParam), HIWORD(lParam), &x1, &y1);
			ZuiDefaultControlProc(Control, ZM_LBUTTONDBCLK, x1, y1);
			break;
		}
		case WM_RBUTTONDOWN: {
			/*鼠标右键被按下*/
			Control = ZuiFindControlInWindow(Window, LOWORD(lParam), HIWORD(lParam), &x1, &y1);
			Window->ButtonDownControl[1] = Control;
			if (Control && Control->Focusable && Window->FocusControl != Control) {
				ZuiControl OldControl = Window->FocusControl;
				Window->FocusControl = Control;
				if (OldControl)
				{
					/*向旧控件发送失去焦点的消息*/
					ZuiDefaultControlProc(OldControl, ZM_KILLFOCUS, 0, (ZuiInt)Control);
				}
				/*向新控件发送得到焦点的消息*/
				ZuiDefaultControlProc(Control, ZM_SETFOCUS, 0, (ZuiInt)OldControl);
			}
			ZuiDefaultControlProc(Control,ZM_RBUTTONDOWN, x1, y1);
			break;
		}
		case WM_RBUTTONUP: {
			/*鼠标右键被弹起*/
			if (Window->CaptureControl)
			{/*捕获消息*/
				ZuiDefaultControlProc(Window->CaptureControl, ZM_RBUTTONUP, LOWORD(lParam) - Window->CaptureControl->Rect.Left, HIWORD(lParam) - Window->CaptureControl->Rect.Top);
				break;
			}
			Control = ZuiFindControlInWindow(Window, LOWORD(lParam), HIWORD(lParam), &x1, &y1);
			ZuiDefaultControlProc(Control, ZM_RBUTTONUP, x1, y1);
			if (Window->ButtonDownControl[0] == Control) {
				/*发送单击事件*/
				ZuiDefaultControlProc(Control, ZM_RBUTTONCLK, x1, y1);
			}
			Window->ButtonDownControl[1] = NULL;
			break;
		}
		case WM_RBUTTONDBLCLK: {
			/*鼠标右键被双击*/
			Control = ZuiFindControlInWindow(Window, LOWORD(lParam), HIWORD(lParam), &x1, &y1);
			ZuiDefaultControlProc(Control, ZM_RBUTTONDBCLK, x1, y1);
			break;
		}
		case WM_MBUTTONDOWN: {
			/*鼠标中键被按下*/
			Control = ZuiFindControlInWindow(Window, LOWORD(lParam), HIWORD(lParam), &x1, &y1);
			Window->ButtonDownControl[2] = Control;
			if (Control && Control->Focusable && Window->FocusControl != Control) {
				ZuiControl OldControl = Window->FocusControl;
				Window->FocusControl = Control;
				if (OldControl)
				{
					/*向旧控件发送失去焦点的消息*/
					ZuiDefaultControlProc(OldControl, ZM_KILLFOCUS, 0, (ZuiInt)Control);
				}
				/*向新控件发送得到焦点的消息*/
				ZuiDefaultControlProc(Control, ZM_SETFOCUS, 0, (ZuiInt)OldControl);
			}
			ZuiDefaultControlProc(Control, ZM_MBUTTONDOWN, x1, y1);
			break;
		}
		case WM_MBUTTONUP: {
			/*鼠标中键被弹起*/
			Control = ZuiFindControlInWindow(Window, LOWORD(lParam), HIWORD(lParam), &x1, &y1);
			ZuiDefaultControlProc(Control, ZM_MBUTTONUP, x1, y1);
			if (Window->ButtonDownControl[0] == Control) {
				/*发送单击事件*/
				ZuiDefaultControlProc(Control, ZM_MBUTTONCLK, x1, y1);
			}
			Window->ButtonDownControl[2] = NULL;
			break;
		}
		case WM_MBUTTONDBLCLK: {
			/*鼠标中键被双击*/
			Control = ZuiFindControlInWindow(Window, LOWORD(lParam), HIWORD(lParam), &x1, &y1);
			ZuiDefaultControlProc(Control, ZM_MBUTTONDBCLK, x1, y1);
			break;
		}
		case WM_MOUSEWHEEL: {
			/*鼠标滚动消息*/
			if (Window->FocusControl) {
				ZuiDefaultControlProc(Window->FocusControl, ZM_MOUSEWHEEL, (ZuiInt)(HIWORD(wParam) / 120), 0);
			}
			break;
		}
		case WM_KEYDOWN: {
			if (Window->FocusControl) {
				ZuiDefaultControlProc(Window->FocusControl, ZM_KEYDOWN, (ZuiInt)wParam, 0);
			}
			break;
		}
		case WM_KEYUP: {
			if (Window->FocusControl) {
				ZuiDefaultControlProc(Window->FocusControl, ZM_KEYUP, (ZuiInt)wParam, 0);
			}
			break;
		}
		case WM_CHAR: {
			if (Window->FocusControl) {
				ZuiDefaultControlProc(Window->FocusControl, ZM_CHAR, (ZuiInt)wParam, 0);
			}
			break;
		}
		case WM_NCHITTEST:{
			if (Window->SizeTunable)
			{
				int x = LOWORD(lParam);
				int	y = HIWORD(lParam);
				if (x <= Window->Rect.Left + 3 && y <= Window->Rect.Top + 3){
					return HTTOPLEFT;
				}
				else if (x <= Window->Rect.Left + 3 && y >= Window->Rect.Top + Window->Rect.Height - 3)
				{
					return HTBOTTOMLEFT;
				}
				else if (x >= Window->Rect.Left + Window->Rect.Width - 3 && y <= Window->Rect.Top + 3)
				{
					return HTTOPRIGHT;
				}
				else if (x >= Window->Rect.Left + Window->Rect.Width - 3 && y >= Window->Rect.Top + Window->Rect.Height - 3)
				{
					return HTBOTTOMRIGHT;
				}
				else if (x <= Window->Rect.Left + 2)
				{
					return HTLEFT;
				}
				else if (y <= Window->Rect.Top + 2)
				{
					return HTTOP;
				}
				else if (x >= Window->Rect.Left + Window->Rect.Width - 2)
				{
					return HTRIGHT;
				}
				else if (y >= Window->Rect.Top + Window->Rect.Height - 2)
				{
					return HTBOTTOM;
				}
				else
				{
					return HTCLIENT;
				}
				return HTCLIENT;
			}
			break;
		}
		default:
			if (Window->FocusControl){
				ZuiDefaultControlProc(Window->FocusControl, ZM_WNDMSG, uMsg, (ZuiInt)wParam);
			}
			break;
	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
#endif


