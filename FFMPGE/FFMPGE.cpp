#pragma comment(lib, "libavcodec.a")
#pragma comment(lib, "libavformat.a")
#pragma comment(lib, "libavutil.a")
#pragma comment(lib, "libswscale.a")
#pragma comment(lib, "libswresample.a")

#include "ui\zui.h"

#include "stdafx.h"
#include "libav\avplay.h"
#include "audio\audio_out.h"
#include "video\video_out.h"
#include "libav\globals.h"
#include "include\DLBT.h"
// 渲染模式.
#define RENDER_DDRAW	0
#define RENDER_D3D		1
#define RENDER_SOFT		3

/* 文件下载信息. */
typedef struct download_bt
{
	HANDLE fd;
	HANDLE bt;
	int num;
	int size;
	bool * arry;
	int ok;
	int64_t offset;
	int whence;
} download_bt;

int file_init_source(struct source_context *ctx)
{
	download_bt *p = (download_bt*)malloc(sizeof(download_bt));
	memset(p, 0, sizeof(download_bt));
	ctx->priv = p;
	wchar_t a[256];
	int b=256;
	p->bt = DLBT_Downloader_Initialize(L"1.torrent", L".\\", L"./a.DLBT", FILE_ALLOCATE_REVERSED, false, false, "", L"", false, false);
	p->num = DLBT_Downloader_GetPieceCount(p->bt);//分块数量
	p->size = DLBT_Downloader_GetPieceSize(p->bt);//分块大小 字节
	p->arry = (bool *)malloc(p->num * sizeof(bool));//分块表
	memset(p->arry, 0, p->num * sizeof(bool));
	DLBT_Downloader_GetFilePathName(p->bt, 0, a, &b, true);
	DLBT_Downloader_SetPiecePrioritize(p->bt, 0, DLBT_FILE_PRIORITY_MAX, TRUE);
	Sleep(1000);
	p->fd = CreateFileW(a, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);//打开文件
	return 0;
}
int64_t file_read_data(struct source_context *ctx, char* buff, size_t buf_size)
{
	download_bt *p = (download_bt*)ctx->priv;
	while (!p->arry[0])
	{
		DLBT_Downloader_GetPiecesStatus(p->bt, p->arry, p->num, &p->ok);//分块状态表
	}
	int aa = buf_size/p->size;
	if (p->offset == SEEK_SET)
	{
		
	}
	DLBT_Downloader_SaveStatusFile(p->bt);
	DWORD size;
	ReadFile(p->fd, buff, buf_size, &size, 0);

	return size;
}
int64_t file_read_seek(struct source_context *ctx, int64_t offset, int whence)
{
	download_bt *p = (download_bt*)ctx->priv;

	p->offset = offset;
	p->whence = whence;
	int64_t a = -1;
	LARGE_INTEGER li = { 0 }; //记得初始化
	li.QuadPart = offset/*移动的位置*/;
	switch (whence)
	{
	case SEEK_SET:	// 文件起始位置计算.
	{
		a = SetFilePointer(p->fd, li.LowPart, &li.HighPart, FILE_BEGIN);
	}
		break;
	case SEEK_CUR:	// 文件指针当前位置开始计算.
	{
		a = SetFilePointer(p->fd, li.LowPart, &li.HighPart, FILE_CURRENT);
	}
		break;
	case SEEK_END:	// 文件尾开始计算.
	{
		a =  SetFilePointer(p->fd, li.LowPart, &li.HighPart, FILE_END);
						 
	}
		break;
	default:
		break;
	}
	return a;
}
void file_close(struct source_context *ctx)
{

}
void file_destory(struct source_context *ctx)
{

}
void init_file_source(source_context *sc)
{
	sc->init_source = file_init_source;
	sc->read_data = file_read_data;
	sc->read_seek = file_read_seek;
	sc->close = file_close;
	sc->destory = file_destory;
}

void init_audio(ao_context *ao)
{

	ao->init_audio = dsound_init_audio;
	ao->play_audio = dsound_play_audio;
	ao->audio_control = dsound_audio_control;
	ao->mute_set = dsound_mute_set;
	ao->destory_audio = dsound_destory_audio;
}
void init_video(vo_context *vo, int render_type/* = RENDER_SOFT*/)
{
	int ret = 0;
	int check = 0;

	do
	{
		if (render_type == RENDER_D3D || check == -1)
		{
			ret = d3d_init_video(vo, 10, 10, PIX_FMT_YUV420P);
			d3d_destory_render(vo);
			if (ret == 0)
			{
				vo->init_video = d3d_init_video;
				vo->re_size = d3d_re_size;
				vo->aspect_ratio = d3d_aspect_ratio;
				vo->use_overlay = d3d_use_overlay;
				vo->destory_video = d3d_destory_render;
				vo->render_one_frame = d3d_render_one_frame;
				break;
			}
		}
		if (render_type == RENDER_DDRAW || check == -1)
		{
			ret = ddraw_init_video(vo, 10, 10, PIX_FMT_YUV420P);
			ddraw_destory_render(vo);
			if (ret == 0)
			{
				vo->init_video = ddraw_init_video;
				vo->re_size = ddraw_re_size;
				vo->aspect_ratio = ddraw_aspect_ratio;
				vo->use_overlay = ddraw_use_overlay;
				vo->destory_video = ddraw_destory_render;
				vo->render_one_frame = ddraw_render_one_frame;
				break;
			}
		}
		if (render_type == RENDER_SOFT || check == -1)
		{
			ret = gdi_init_video(vo, 10, 10, PIX_FMT_YUV420P);
			gdi_destory_render(vo);
			if (ret == 0)
			{
				vo->init_video = gdi_init_video;
				vo->re_size = gdi_re_size;
				vo->aspect_ratio = gdi_aspect_ratio;
				vo->use_overlay = gdi_use_overlay;
				vo->destory_video = gdi_destory_render;
				vo->render_one_frame = gdi_render_one_frame;
				break;
			}
		}
	} while (check-- == 0);

	// 表示视频渲染器初始化失败!!!
	assert(check != -2);
}

avplay *m_avplay;//
ao_context *m_audio;//音频渲染器
vo_context *m_video;//视频渲染器
source_context *m_source;//媒体源
ZuiWindow Window;

LRESULT CALLBACK bt_play_voide_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_LBUTTONUP)
	{
	//	av_seek(m_avplay, LOWORD(lparam) / 8 * 0.01);
		
		return 0;
	}
		return DefWindowProc(hwnd, msg, wparam, lparam);
}



int _tmain(int argc, _TCHAR* argv[])
{
	DLBT_KERNEL_START_PARAM param;
	param.startPort = 9010;     // 尝试绑定9010端口，如果9010端口未被占用，则使用。否则，继续尝试下一个端口，直到endPort指定的范围。
	param.endPort = 9030;
	if (!DLBT_Startup(&param, "", false, "{11111111-1111-1111-1111-111111111111}")){//初始化点量BT内核
		return 0;
	}
	DLBT_DHT_Start();//启动DHT网络
	DLBT_SetEncryptSetting(DLBT_ENCRYPT_FULL, DLBT_ENCRYPT_ALL);// 默认支持加密传输

	if (!ZuiInit()){//初始化UI库
		return 0;
	}

	Window = ZuiCreateWindow(100, 100, 600, 500, VWS_TITLE | VWS_POSMIDDLE | VWS_CTLBTN_MIN, L"BT播放器",
		ZuiCreateStringFormat(L"微软雅黑", 12, ARGB(255, 0, 0, 0), ARGB(255, 255, 255, 255), VTS_VALIGN_TOP),
		NULL, ARGB(255, 0, 255, 255), 0, VWFS_RECT);//创建主窗口
	ZuiSetSizeTunable(Window, TRUE);//设置窗口大小可调

	// 创建播放窗口.
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_CLASSDC/*CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS*/;
	wcex.lpfnWndProc = &bt_play_voide_proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 1));	// (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;				// MAKEINTRESOURCE(IDC_AVPLAYER);
	wcex.lpszClassName = L"bt_play_voide";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDC_ICON));
	if (!RegisterClassEx(&wcex))
		return NULL;
	HWND m_hwnd = CreateWindowEx(/*WS_EX_APPWINDOW*/0,
		L"bt_play_voide", L"", WS_CHILDWINDOW,
		5, 25, 500, Window->Rect.Height - 25 - 50, Window->OsWindow->hWnd, NULL, GetModuleHandle(NULL), NULL);
	ShowWindow(m_hwnd, SW_SHOW);

	ZuiControl Button1 = ZuiButtonCreate(ZuiGetWindowRootControl(Window), 100, 450, 50, 40, L"播放", ZuiCreateStringFormat(L"微软雅黑", 12, ARGB(255, 0, 0, 0), ARGB(255, 255, 255, 255), VTS_ALIGN_CENTER | VTS_VALIGN_MIDDLE | VTS_SHADOW), TRUE, TRUE);
	ZuiControl Button2 = ZuiButtonCreate(ZuiGetWindowRootControl(Window), 50, 450, 50, 40, L"上一集", ZuiCreateStringFormat(L"微软雅黑", 12, ARGB(255, 0, 0, 0), ARGB(255, 255, 255, 255), VTS_ALIGN_CENTER | VTS_VALIGN_MIDDLE | VTS_SHADOW), TRUE, TRUE);
	ZuiControl Button3 = ZuiButtonCreate(ZuiGetWindowRootControl(Window), 150, 450, 50, 40, L"下一集", ZuiCreateStringFormat(L"微软雅黑", 12, ARGB(255, 0, 0, 0), ARGB(255, 255, 255, 255), VTS_ALIGN_CENTER | VTS_VALIGN_MIDDLE | VTS_SHADOW), TRUE, TRUE);
	ZuiColor ButtonColor1[] = { ARGB(0, 0, 0, 0), ARGB(255, 255, 255, 0), ARGB(255, 0, 255, 0), ARGB(196, 123, 123, 123), ARGB(196, 255, 255, 255) };
	ZuiButtonSetStyle_Metro(Button1, ButtonColor1);
	ZuiButtonSetStyle_Metro(Button3, ButtonColor1);
	ZuiButtonSetStyle_Metro(Button2, ButtonColor1);
	ZuiPlayListCreate(ZuiGetWindowRootControl(Window), 520, 25, 50, Window->Rect.Height, TRUE, TRUE);

	ZuiRedrawWindow(Window);//刷新窗口
	ZuiSetWindowVisible(Window, TRUE);//窗口可视

	/*
	// 分配音频和视频的渲染器.
	m_audio = alloc_audio_render();
	m_video = alloc_video_render(m_hwnd);
	// 初始化音频和视频渲染器.
	init_audio(m_audio);
	init_video(m_video, RENDER_D3D);

	// 分配播放器上下文.
	m_avplay = alloc_avplay_context();
	//分配媒体源
	m_source = alloc_media_source(MEDIA_TYPE_FILE, "E:\MVI_3981.MOV", 6 + 1, 1000000);
	// 初始化文件媒体源.
	init_file_source(m_source);
	// 初始化avplay.
	initialize(m_avplay, m_source);
	// 配置音频视频渲染器.
	configure(m_avplay, m_video, VIDEO_RENDER);
	configure(m_avplay, m_audio, AUDIO_RENDER);

	av_start(m_avplay, 0);
	*/

	return ZuiMsgLoop();//进入消息循环


	
	/*
	
	int ok = 0;									//已下载大小
	memset(arry, 0, num*sizeof(bool));
	SetTimer(0, 0, 1000, 0);
	DLBT_Downloader_GetPiecesStatus(bt, arry, num, &ok);//分块状态表
	printf("%d\n", ok);

	enable_calc_frame_rate(m_avplay);
	//等待线程初始化好声卡设备
	Sleep(1000);
	//av_volume(m_avplay, 10, 10);
	return 0;
	*/
}

