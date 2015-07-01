#include "gdi.h"

#if (defined PLATFORM_GRAPH_GDI) && (PLATFORM_GRAPH_GDI == 1) && (defined PLATFORM_OS_WIN)

typedef struct
{
	ZuiInt GdiplusVersion;
	ZuiInt DebugEventCallback;
	ZuiInt SuppressBackgroundThreadc;
	ZuiInt SuppressExternalCodecs;
}GdiplusStartupInput;
#ifdef __cplusplus
extern "C" {
#endif
	int __stdcall GdiplusStartup(int *token, GdiplusStartupInput *input, void *output);
	int __stdcall GdipCreateFromHDC(HDC hdc, void **graphics);
	int __stdcall GdipSetTextRenderingHint(void *graphics, int a);
	int __stdcall GdipDeleteGraphics(void *graphics);
	int __stdcall GdipCreateSolidFill(ZuiColor color, void **brush);
	int __stdcall GdipFillRectangleI(void *graphics, void *brush, int x, int y, int width, int height);
	int __stdcall GdipDeleteBrush(void *brush);
	int __stdcall GdipDrawRectangleI(void *graphics, void *pen, int x, int y, int width, int height);
	int __stdcall GdipCreatePen1(ZuiColor color, ZuiReal width, int unit, void **pen);
	int __stdcall GdipDeletePen(void *pen);
	int __stdcall GdipDrawLineI(void *graphics, void *pen, ZuiInt x1, ZuiInt y1, ZuiInt x2, ZuiInt y2);
	int __stdcall GdipDrawString(void *graphics, ZuiText string, int length, void *font, ZuiRectR layoutRect, void *stringFormat, void *brush);
	int __stdcall GdipMeasureString(void *graphics, ZuiText string, ZuiInt length, void *font, ZuiRectR layoutRect, void *stringFormat, ZuiRectR boundingBox, ZuiInt codepointsFitted, ZuiInt linesFilled);
	int __stdcall GdipDrawImageI(void *graphics, void *image, int x, int y);
	int __stdcall GdipGraphicsClear(void *graphics, ZuiColor color);
	int __stdcall GdipCreateFontFamilyFromName(ZuiText name, void *fontCollection, void **fontFamily);
	int __stdcall GdipDeleteFontFamily(void *fontFamily);
	int __stdcall GdipCreateFont(void *fontFamily, ZuiReal emSize, int style, int unit, void **font);
	int __stdcall GdipCreateStringFormat(int formatAttributes, int language, void **StringFormat);
	int __stdcall GdipSetStringFormatAlign(void *format, int align);
	int __stdcall GdipSetStringFormatLineAlign(void *format, int align);
	int __stdcall GdipDeleteFont(void* font);
	int __stdcall GdipLoadImageFromStream(void* stream, void **image);
	int __stdcall GdipLoadImageFromFile(ZuiText filename, void **image);
	int __stdcall GdipGetImageWidth(void *image, int *width);
	int __stdcall GdipGetImageHeight(void *image, int *height);
	int __stdcall GdipDisposeImage(void *image);
	int __stdcall GdipDrawImageRectRectI(void *graphics, void *image, int dstx, int dsty, int dstwidth, int dstheight, int srcx, int srcy, int srcwidth, int srcheight, int srcUnit, void* imageAttributes, void *callback, void * callbackData);
	int __stdcall CreateStreamOnHGlobal(HGLOBAL hGlobal, BOOL fDeleteOnRelease, void **ppstm);
#ifdef __cplusplus
}
#endif

ZRectR ZeroRectR;//零坐标矩形

ZuiBool ZuiGraphInitialize(){
	GdiplusStartupInput gdiplusStartupInput;
	ZuiInt pGdiToken;
	memset(&gdiplusStartupInput, 0, sizeof(GdiplusStartupInput));
	gdiplusStartupInput.GdiplusVersion = 1;
	GdiplusStartup(&pGdiToken, &gdiplusStartupInput, NULL);//初始化GDI+
	MAKEVRECT(ZeroRectR, 0, 0, 0, 0);
	return TRUE;
}

ZAPI(ZuiVoid) ZuiFillRect(ZuiGraphics Graphics, ZuiColor Color, ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height) {
	void *Brush;
	GdipCreateSolidFill(Color, &Brush);
	GdipFillRectangleI(Graphics->graphics, Brush, Left, Top, Width, Height);
	GdipDeleteBrush(Brush);
}

ZAPI(ZuiVoid) ZuiDrawFilledRect(ZuiGraphics Graphics, ZuiColor Color, ZuiColor BorderColor, ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height, ZuiInt LineWidth) {
}

ZAPI(ZuiVoid) ZuiDrawRect(ZuiGraphics Graphics, ZuiColor Color, ZuiInt Left, ZuiInt Top, ZuiInt Width, ZuiInt Height, ZuiInt LineWidth) {
	void *pen;
	GdipCreatePen1(Color, (ZuiReal)LineWidth, 2, &pen);
	GdipDrawRectangleI(Graphics->graphics, pen, Left, Top, Width, Height);
	GdipDeletePen(pen);
}

ZAPI(ZuiVoid) ZuiDrawLine(ZuiGraphics Graphics, ZuiColor Color, ZuiInt x1, ZuiInt y1, ZuiInt x2, ZuiInt y2, ZuiInt LineWidth)
{
	void *pen;
	GdipCreatePen1(Color, (ZuiReal)LineWidth, 2, &pen);
	GdipDrawLineI(Graphics->graphics, pen, x1, y1, x2, y2);
	GdipDeletePen(pen);
}

ZAPI(ZuiVoid) ZuiDrawString(ZuiGraphics Graphics, ZuiStringFormat StringFormat, ZuiText String, ZuiRect Rect) {
	ZRectR r;
	MAKEVRECT(r, (ZuiReal)Rect->Left, (ZuiReal)Rect->Top, (ZuiReal)Rect->Width, (ZuiReal)Rect->Height);
	GdipDrawString(Graphics->graphics, String, wcslen(String), StringFormat->font, &r, StringFormat->StringFormat, StringFormat->Brush);
}

ZAPI(ZuiVoid) ZuiMeasureStringRect(ZuiGraphics Graphics, ZuiStringFormat StringFormat, ZuiText String, ZuiRectR Rect)
{
	int l = wcslen(String);
	GdipMeasureString(Graphics->graphics, String, wcslen(String), StringFormat->font, &ZeroRectR, StringFormat->StringFormat, Rect, 0, 0);
}


/*
ZAPI(ZuiVoid) ZuiCreateRoundRectPath(ZuiInt x, ZuiInt y, ZuiInt Width, ZuiInt Height, ZuiReal Round, SkPath* Path) {
#ifdef DRAW_SKIA
Path->addArc(SkRect::MakeXYWH((ZuiReal)x, (ZuiReal)y, Round, Round), 180, 90);
Path->addArc(SkRect::MakeXYWH((ZuiReal)(x + Width - Round), (ZuiReal)y, Round, Round), 270, 90);
Path->addArc(SkRect::MakeXYWH((ZuiReal)(x + Width - Round), (ZuiReal)(y + Height - Round), Round, Round), 0, 90);
Path->addArc(SkRect::MakeXYWH((ZuiReal)x, (ZuiReal)(y + Height - Round), Round, Round), 90, 90);
Path->close();
#elif defined DRAW_GDI

#endif
}
*/
ZAPI(ZuiVoid) ZuiDrawFilledRoundRect(ZuiGraphics Graphics, ZuiColor Color, ZuiColor BorderColor, ZuiInt x, ZuiInt y, ZuiInt Width, ZuiInt Height, ZuiInt LineWidth, ZuiReal Round) {

}

ZAPI(ZuiVoid) ZuiDrawRoundRect(ZuiGraphics Graphics, ZuiColor Color, ZuiInt x, ZuiInt y, ZuiInt Width, ZuiInt Height, ZuiInt LineWidth, ZuiReal Round) {
	if (Round <= 0){
		ZuiDrawRect(Graphics, Color, x, y, Width, Height, LineWidth);
		return;
	}

}

ZAPI(ZuiVoid) ZuiFillRoundRect(ZuiGraphics Graphics, ZuiColor Color, ZuiInt x, ZuiInt y, ZuiInt Width, ZuiInt Height, ZuiReal Round) {

}

ZAPI(ZuiVoid) ZuiDrawImage(ZuiGraphics Graphics, ZuiImage Image, ZuiInt x, ZuiInt y) {
	if (!(Graphics && Image)) {
		return;
	}
	GdipDrawImageI(Graphics->graphics, Image->image, x, y);
}

ZAPI(ZuiVoid) ZuiDrawImageEx(ZuiGraphics Graphics, ZuiImage Image, ZuiInt x, ZuiInt y, ZuiInt Width, ZuiInt Height, ZuiInt xSrc, ZuiInt ySrc, ZuiInt WidthSrc, ZuiInt HeightSrc, ZuiByte Alpha) {
	if (!(Graphics && Image)) {
		return;
	}

	if (Width <= 0) {
		Width = (ZuiInt)(Image->Width - xSrc);
	}
	if (Height <= 0) {
		Height = (ZuiInt)(Image->Height - ySrc);
	}


	if (WidthSrc <= 0 || WidthSrc + xSrc > Image->Width) {
		WidthSrc = (ZuiInt)(Image->Width - xSrc);
	}
	if (HeightSrc <= 0 || HeightSrc + ySrc > Image->Height) {
		HeightSrc = (ZuiInt)(Image->Height - ySrc);
	}
	GdipDrawImageRectRectI(Graphics->graphics, Image->image, x, y, Width, Height, xSrc, ySrc, WidthSrc, HeightSrc, 2, NULL, NULL, NULL);
}

ZAPI(ZuiVoid) ZuiAlphaBlendEx(ZuiGraphics Dest, ZuiInt x, ZuiInt y, ZuiInt Width, ZuiInt Height, ZuiGraphics Src, ZuiInt xSrc, ZuiInt ySrc, ZuiInt WidthSrc, ZuiInt HeightSrc, ZuiByte Alpha) {
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 1;
	bf.SourceConstantAlpha = Alpha;
	int a = AlphaBlend(Dest->hdc, x, y, Width, Height, Src->hdc, xSrc, ySrc, WidthSrc, HeightSrc, bf);
}

ZAPI(ZuiVoid) ZuiAlphaBlend(ZuiGraphics Dest, ZuiInt x, ZuiInt y, ZuiInt Width, ZuiInt Height, ZuiGraphics Src, ZuiInt xSrc, ZuiInt ySrc, ZuiByte Alpha) {
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 1;
	bf.SourceConstantAlpha = Alpha;
	int a = AlphaBlend(Dest->hdc, x, y, Width, Height, Src->hdc, xSrc, ySrc, (Src->Width > Width) ? Width : Src->Width, (Src->Height > Height) ? Height : Src->Height, bf);
}

ZAPI(ZuiVoid) ZuiGraphicsClear(ZuiGraphics Graphics, ZuiColor Color) {
	GdipGraphicsClear(Graphics->graphics, Color);
}

ZAPI(ZuiStringFormat) ZuiCreateStringFormat(ZuiText FontName, ZuiReal FontSize, ZuiColor TextColor, ZuiColor ShadowColor, ZuiInt StringStyle) {
	int i = 0;
	ZuiStringFormat StringFormat = (ZuiStringFormat)malloc(sizeof(ZStringFromat));
	if (!StringFormat){ return NULL; }
	memset(StringFormat, 0, sizeof(ZStringFromat));

	GdipCreateStringFormat(0, 0, &StringFormat->StringFormat);//创建字体格式
	int a = GetLastError();
	if (StringStyle & VTS_ALIGN_LEFT){
		i = 0;
	}
	else if (StringStyle & VTS_ALIGN_CENTER){
		i = 1;
	}
	else if (StringStyle & VTS_ALIGN_RIGHT)
	{
		i = 2;
	}
	GdipSetStringFormatAlign(StringFormat->StringFormat, i);//设置水平对齐方式
	if (StringStyle & VTS_VALIGN_TOP){
		i = 0;
	}
	else if (StringStyle & VTS_VALIGN_MIDDLE){
		i = 1;
	}
	else if (StringStyle & VTS_VALIGN_BOTTOM)
	{
		i = 2;
	}
	GdipSetStringFormatLineAlign(StringFormat->StringFormat, i);//设置纵向对齐方式
	i = 0;
	if (StringStyle & VTS_BOLD){
		i = 1;
	}
	else if (StringStyle & VTS_ITALIC){
		i += 2;
	}

	GdipCreateFontFamilyFromName(FontName, NULL, &StringFormat->FontFamily);
	GdipCreateFont(StringFormat->FontFamily, FontSize, i, 0, &StringFormat->font);
	GdipDeleteFontFamily(StringFormat->FontFamily);
	if (StringStyle & VTS_SHADOW)
	{
		GdipCreateSolidFill(ShadowColor, &StringFormat->BrushShadow);
	}
	GdipCreateSolidFill(TextColor, &StringFormat->Brush);
	StringFormat->TextColor = TextColor;
	return StringFormat;
}

ZAPI(ZuiVoid) ZuiDestroyStringFormat(ZuiStringFormat StringFormat) {
	if (StringFormat){
		GdipDeleteFontFamily(StringFormat->FontFamily);
		GdipDeleteFont(StringFormat->font);
		GdipDeleteBrush(StringFormat->Brush);
		free(StringFormat);
	}
}

ZAPI(ZuiGraphics) ZuiCreateGraphicsInMemory(ZuiInt Width, ZuiInt Height) {
	ZuiGraphics Graphics = (ZuiGraphics)malloc(sizeof(ZGraphics));
	if (!Graphics){ return NULL; }
	memset(Graphics, 0, sizeof(ZGraphics));
	Graphics->Width = Width;
	Graphics->Height = Height;

	Graphics->hdc = CreateCompatibleDC(NULL);
	BITMAPINFO BitmapInfo;
	memset(&BitmapInfo, 0, sizeof(BitmapInfo));
	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biWidth = Width;
	BitmapInfo.bmiHeader.biHeight = Height;
	BitmapInfo.bmiHeader.biPlanes = 1;
	void* Bits;
	Graphics->HBitmap = CreateDIBSection(0, &BitmapInfo, 0, &Bits, 0, 0);
	if (!Graphics->HBitmap) {
		DeleteDC(Graphics->hdc);
		return NULL;
	}
	HBITMAP OldBitmap = (HBITMAP)SelectObject(Graphics->hdc, (HGDIOBJ)Graphics->HBitmap);

	GdipCreateFromHDC(Graphics->hdc, &Graphics->graphics);
	GdipSetTextRenderingHint(Graphics->graphics, 3);//设置抗锯齿模式
	return Graphics;
}


ZAPI(ZuiVoid) ZuiDestroyGraphics(ZuiGraphics Graphics) {
	if (Graphics){
		GdipDeleteGraphics(Graphics);
		DeleteDC(Graphics->hdc);
		DeleteObject(Graphics->HBitmap);
		free(Graphics);
	}
}

ZAPI(ZuiImage) ZuiLoadImageFromFile(ZuiText FileName) {
	ZuiImage Image = (ZuiImage)malloc(sizeof(ZImage));
	if (!Image){ return NULL; }
	memset(Image, 0, sizeof(ZImage));
	GdipLoadImageFromFile(FileName, &Image->image);
	GdipGetImageHeight(Image->image, &Image->Height);
	GdipGetImageWidth(Image->image, &Image->Width);
	return Image;
}

ZAPI(ZuiImage) ZuiLoadImageFromBinary(ZuiBin Binary) {
	ZuiImage Image = (ZuiImage)malloc(sizeof(ZImage));
	if (!Image){ return NULL; }
	memset(Image, 0, sizeof(ZImage));
	HGLOBAL hMem = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE, Binary->Length);
	void *str;
	CreateStreamOnHGlobal(hMem, 0, &str);
	void * mem = GlobalLock(hMem);
	memcpy(mem, Binary->Address, Binary->Length);
	GlobalUnlock(hMem);
	GdipLoadImageFromStream(str, &Image->image);
	GdipGetImageHeight(Image->image, &Image->Height);
	GdipGetImageWidth(Image->image, &Image->Width);
	return Image;
}

ZAPI(ZuiVoid) ZuiDestroyImage(ZuiImage Image) {
	GdipDisposeImage(Image->image);
	free(Image);
}


#endif

