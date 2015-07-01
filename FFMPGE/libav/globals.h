#ifndef __AVPLAYER_GLOBALS_H__
#define __AVPLAYER_GLOBALS_H__

#include <stdint.h>

#include "ffmpeg.h"

/* 媒体数据源接口. */
#define MEDIA_TYPE_FILE	0
#define MEDIA_TYPE_BT	1
#define MEDIA_TYPE_HTTP 2
#define MEDIA_TYPE_RTSP 3

#ifndef MAX_URI_PATH
#define MAX_URI_PATH	2048
#endif

/* 文件下载信息. */
typedef struct download_info
{
	int speed;			/* 下载下载速率. */
	int limit_speed;	/* 限制下载速率. */
	int not_enough;		/* 若为1, 则表示数据不够, 需要缓冲, 若为0, 表示缓冲完成. */
} download_info;

/* 媒体数据源接口. */
typedef struct source_context
{
	/* init_source 函数用于初始化数据source部件, 因为视频数据的来源可能不同, 则具体实现也可能不同, 所以, 初化时也会不同*/
	int (*init_source)(struct source_context *source_ctx);
	/* read_data 函数用于读取指定大小的数据, 返回已经读取的数据大小*/
	int64_t (*read_data)(struct source_context *source_ctx, char* buff, size_t buf_size);
	/* 跳转读取数据的位置*/
	int64_t (*read_seek)(struct source_context *source_ctx, int64_t offset, int whence);
	/* 关闭数据读取源*/
	void (*close)(struct source_context *source_ctx);
	/* 销毁数据读取源*/
	void (*destory)(struct source_context *source_ctx);

	/* priv是保存内部用于访问实际数据的对象指针.
	 */
	void *priv;
	/* 数据类型, 可以是以下值
	 * MEDIA_TYPE_FILE
	 * MEDIA_TYPE_BT
	 * MEDIA_TYPE_HTTP
	 * MEDIA_TYPE_RTSP
	 * 说明: 由于未实现http和rtsp协议而是直接使用了ffmpeg的demux, 所以就无需初始
	 * 化上面的各函数指针.
	 */
	int type;
	char name[MAX_URI_PATH];
	/* 用于获得和控制下载.*/
	download_info dl_info;
	/*当前退出标识, 退出时为true.*/
	int abort;
} source_context;

/*视频播放结构定义*/
typedef struct vo_context
{
	int (*init_video)(struct vo_context *vo_ctx, int w, int h, int pix_fmt);
	int (*render_one_frame)(struct vo_context *vo_ctx, AVFrame* data, int pix_fmt, double pts);
	void (*re_size)(struct vo_context *vo_ctx, int width, int height);
	void (*aspect_ratio)(struct vo_context *vo_ctx, int srcw, int srch, int enable_aspect);
	int (*use_overlay)(struct vo_context *vo_ctx);
	void (*destory_video)(struct vo_context *vo_ctx);
	void *priv;
	void *user_data;	/* for window hwnd. */
	void *user_ctx;		/* for user context. */
	float fps;			/* fps */
} vo_context;
/*音频播放输出结构定义*/
typedef struct ao_context
{
	int (*init_audio)(struct ao_context *ao_ctx, uint32_t channels, uint32_t bits_per_sample,
		uint32_t sample_rate, int format);
	int (*play_audio)(struct ao_context *ao_ctx, uint8_t *data, uint32_t size);
	void (*audio_control)(struct ao_context *ao_ctx, double l, double r);
	void (*mute_set)(struct ao_context *ao_ctx, int s);
	void (*destory_audio)(struct ao_context *ao_ctx);
	void *priv;
} ao_context;

#endif /* __AVPLAYER_GLOBALS_H__ */
