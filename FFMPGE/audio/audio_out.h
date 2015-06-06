#ifndef __AUDIO_OUT_H__
#define __AUDIO_OUT_H__

#ifdef _MSC_VER
#	ifdef AUDIO_EXPORTS
#		define __declspec(dllexport)
#	else
#		define __declspec(dllimport)
#	endif
#else
#	define EXPORT_API
#endif

#ifdef  __cplusplus
extern "C" {
#endif

int wave_init_audio(struct ao_context *ctx, uint32_t channels, 
	uint32_t bits_per_sample, uint32_t sample_rate, int format);
int wave_play_audio(struct ao_context *ctx, uint8_t *data, uint32_t size);
void wave_audio_control(struct ao_context *ctx, double l, double r);
void wave_mute_set(struct ao_context *ctx, int s);
void wave_destory_audio(struct ao_context *ctx);


int dsound_init_audio(struct ao_context *ctx, uint32_t channels, 
	uint32_t bits_per_sample, uint32_t sample_rate, int format);
int dsound_play_audio(struct ao_context *ctx, uint8_t *data, uint32_t size);
void dsound_audio_control(struct ao_context *ctx, double l, double r);
void dsound_mute_set(struct ao_context *ctx, int s);
void dsound_destory_audio(struct ao_context *ctx);

int sdl_init_audio(struct ao_context *ctx, uint32_t channels,
	uint32_t bits_per_sample, uint32_t sample_rate, int format);
int sdl_play_audio(struct ao_context *ctx, uint8_t *data, uint32_t size);
void sdl_audio_control(struct ao_context *ctx, double l, double r);
void sdl_mute_set(struct ao_context *ctx, int s);
void sdl_destory_audio(struct ao_context *ctx);


#ifdef  __cplusplus
}
#endif

#endif // __AUDIO_OUT_H__


