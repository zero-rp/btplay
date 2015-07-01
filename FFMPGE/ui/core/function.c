#include "function.h"

ZAPI(ZuiBool) ZuiInit() {
	/*初始化移植层*/
	if (!ZuiOsInitialize()) {
		return FALSE;
	}
	/*初始化图形层*/
	if (!ZuiGraphInitialize()){
		return FALSE;
	}
	/*初始化绑定层*/
	//if (!ZuiBuilderInit()){
	//	return FALSE;
	//}
	return TRUE;
}

ZAPI(ZuiInt) ZuiMsgLoop() {
	return ZuiOsMessageLoop();
}

ZuiBool ZuiIsPointInRect(ZuiInt x, ZuiInt y, ZuiRect Rect) {
	if (x <= Rect->Left) {
		return FALSE;
	}
	if (x >= Rect->Width + Rect->Left) {
		return FALSE;
	}

	if (y <= Rect->Top) {
		return FALSE;
	}
	if (y >= Rect->Height + Rect->Top) {
		return FALSE;
	}

	return TRUE;
}
ZuiBool ZuiIsPointInRectR(ZuiReal x, ZuiReal y, ZuiRectR Rect) {
	if (x <= Rect->Left) {
		return FALSE;
	}
	if (x >= Rect->Width + Rect->Left) {
		return FALSE;
	}

	if (y <= Rect->Top) {
		return FALSE;
	}
	if (y >= Rect->Height + Rect->Top) {
		return FALSE;
	}

	return TRUE;
}

ZuiBool ZuiIntersectRect(ZuiRect Rect1, ZuiRect Rect2, ZuiRect RectResult) {
	RectResult->Left = max(Rect1->Left, Rect2->Left);
	RectResult->Top = max(Rect1->Top, Rect2->Top);
	RectResult->Width = min(Rect1->Left + Rect1->Width, Rect2->Left + Rect2->Width) - RectResult->Left;
	RectResult->Height = min(Rect1->Top + Rect1->Height, Rect2->Top + Rect2->Height) - RectResult->Top;
	return (RectResult->Width > 0) && (RectResult->Height > 0);
}

