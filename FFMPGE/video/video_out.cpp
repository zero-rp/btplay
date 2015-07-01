#include "internal.h"
#include "../libav/globals.h"
#include "video_out.h"
#include "d3d_render.h"
#include "ddraw_render.h"
#include "soft_render.h"

#ifdef  __cplusplus
extern "C" {
#endif

int d3d_init_video(struct vo_context *ctx, int w, int h, int pix_fmt)
{
	d3d_render *d3d = NULL;
	ctx->priv = (void*)(d3d = new d3d_render);
	return d3d->init_render(ctx->user_data, w, h, pix_fmt) ? 0 : -1;
}

int d3d_render_one_frame(struct vo_context *ctx, AVFrame* data, int pix_fmt, double pts)
{
	d3d_render *d3d = (d3d_render*)ctx->priv;
	return d3d->render_one_frame(data, pix_fmt) ? 0 : -1;
}

void d3d_re_size(struct vo_context *ctx, int width, int height)
{
	d3d_render *d3d = (d3d_render*)ctx->priv;
	d3d->re_size(width, height);
}

void d3d_aspect_ratio(struct vo_context *ctx, int srcw, int srch, int enable_aspect)
{
	d3d_render *d3d = (d3d_render*)ctx->priv;
	d3d->aspect_ratio(srcw, srch, (bool)enable_aspect);
}

int d3d_use_overlay(struct vo_context *ctx)
{
	d3d_render *d3d = (d3d_render*)ctx->priv;
	return d3d->use_overlay() ? 0 : -1;
}

void d3d_destory_render(struct vo_context *ctx)
{
	d3d_render *d3d = (d3d_render*)ctx->priv;
	if (d3d)
	{
		d3d->destory_render();
		delete d3d;
		ctx->priv = NULL;
	}
}


int ddraw_init_video(struct vo_context *ctx, int w, int h, int pix_fmt)
{
	ddraw_render *ddraw = NULL;
	ctx->priv = (void*)(ddraw = new ddraw_render);
	return ddraw->init_render(ctx->user_data, w, h, pix_fmt) ? 0 : -1;
}

int ddraw_render_one_frame(struct vo_context *ctx, AVFrame* data, int pix_fmt, double pts)
{
	ddraw_render *ddraw = (ddraw_render*)ctx->priv;
	return ddraw->render_one_frame(data, pix_fmt) ? 0 : -1;
}

void ddraw_re_size(struct vo_context *ctx, int width, int height)
{
	ddraw_render *ddraw = (ddraw_render*)ctx->priv;
	ddraw->re_size(width, height);
}

void ddraw_aspect_ratio(struct vo_context *ctx, int srcw, int srch, int enable_aspect)
{
	ddraw_render *ddraw = (ddraw_render*)ctx->priv;
	ddraw->aspect_ratio(srcw, srch, (bool)enable_aspect);
}

int ddraw_use_overlay(struct vo_context *ctx)
{
	ddraw_render *ddraw = (ddraw_render*)ctx->priv;
	return ddraw->use_overlay() ? 0 : -1;
}

void ddraw_destory_render(struct vo_context *ctx)
{
	ddraw_render *ddraw = (ddraw_render*)ctx->priv;
	if (ddraw)
	{
		ddraw->destory_render();
		delete ddraw;
		ctx->priv = NULL;
	}
}

int gdi_init_video(struct vo_context *ctx, int w, int h, int pix_fmt)
{
	soft_render *gdi = new soft_render;

	ctx->priv = (void*)gdi;
	return gdi->init_render(ctx->user_data, w, h, pix_fmt) ? 0 : -1;
}

int gdi_render_one_frame(struct vo_context *ctx, AVFrame* data, int pix_fmt, double pts)
{
	soft_render *gdi = (soft_render*)ctx->priv;
	return gdi->render_one_frame(data, pix_fmt) ? 0 : -1;
}

void gdi_re_size(struct vo_context *ctx, int width, int height)
{
	soft_render *gdi = (soft_render*)ctx->priv;
	gdi->re_size(width, height);
}

void gdi_aspect_ratio(struct vo_context *ctx, int srcw, int srch, int enable_aspect)
{
	soft_render *gdi = (soft_render*)ctx->priv;
	gdi->aspect_ratio(srcw, srch, (bool)enable_aspect);
}

int gdi_use_overlay(struct vo_context *ctx)
{
	soft_render *gdi = (soft_render*)ctx->priv;
	return gdi->use_overlay() ? 0 : -1;
}

void gdi_destory_render(struct vo_context *ctx)
{
	soft_render *gdi = (soft_render*)ctx->priv;
	if (gdi)
	{
		gdi->destory_render();
		delete gdi;
		ctx->priv = NULL;
	}
}

#ifdef  __cplusplus
}
#endif
