#ifndef __VIDEO_OUT_H__
#define __VIDEO_OUT_H__

#ifdef _MSC_VER
#ifdef VIDEO_EXPORTS
#define __declspec(dllexport)
#else
#define __declspec(dllimport)
#endif
#endif

#ifdef  __cplusplus
extern "C" {
#endif

int d3d_init_video(struct vo_context *ctx, int w, int h, int pix_fmt);
int d3d_render_one_frame(struct vo_context *ctx, AVFrame* data, int pix_fmt, double pts);
void d3d_re_size(struct vo_context *ctx, int width, int height);
void d3d_aspect_ratio(struct vo_context *ctx, int srcw, int srch, int enable_aspect);
int d3d_use_overlay(struct vo_context *ctx);
void d3d_destory_render(struct vo_context *ctx);

int ddraw_init_video(struct vo_context *ctx, int w, int h, int pix_fmt);
int ddraw_render_one_frame(struct vo_context *ctx, AVFrame* data, int pix_fmt, double pts);
void ddraw_re_size(struct vo_context *ctx, int width, int height);
void ddraw_aspect_ratio(struct vo_context *ctx, int srcw, int srch, int enable_aspect);
int ddraw_use_overlay(struct vo_context *ctx);
void ddraw_destory_render(struct vo_context *ctx);

int ogl_init_video(struct vo_context *ctx, int w, int h, int pix_fmt);
int ogl_render_one_frame(struct vo_context *ctx, AVFrame* data, int pix_fmt, double pts);
void ogl_re_size(struct vo_context *ctx, int width, int height);
void ogl_aspect_ratio(struct vo_context *ctx, int srcw, int srch, int enable_aspect);
int ogl_use_overlay(struct vo_context *ctx);
void ogl_destory_render(struct vo_context *ctx);

int gdi_init_video(struct vo_context *ctx, int w, int h, int pix_fmt);
int gdi_render_one_frame(struct vo_context *ctx, AVFrame* data, int pix_fmt, double pts);
void gdi_re_size(struct vo_context *ctx, int width, int height);
void gdi_aspect_ratio(struct vo_context *ctx, int srcw, int srch, int enable_aspect);
int gdi_use_overlay(struct vo_context *ctx);
void gdi_destory_render(struct vo_context *ctx);

int y4m_init_video(struct vo_context *ctx, int w, int h, int pix_fmt);
int y4m_render_one_frame(struct vo_context *ctx, AVFrame* data, int pix_fmt, double pts);
void y4m_re_size(struct vo_context *ctx, int width, int height);
void y4m_aspect_ratio(struct vo_context *ctx, int srcw, int srch, int enable_aspect);
int y4m_use_overlay(struct vo_context *ctx);
void y4m_destory_render(struct vo_context *ctx);

int sdl_init_video(struct vo_context *ctx, int w, int h, int pix_fmt);
int sdl_render_one_frame(struct vo_context *ctx, AVFrame* data, int pix_fmt, double pts);
void sdl_re_size(struct vo_context *ctx, int width, int height);
void sdl_aspect_ratio(struct vo_context *ctx, int srcw, int srch, int enable_aspect);
int sdl_use_overlay(struct vo_context *ctx);
void sdl_destory_render(struct vo_context *ctx);

#ifdef  __cplusplus
}
#endif

#endif // __VIDEO_OUT_H__

