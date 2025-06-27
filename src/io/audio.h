#pragma once

#include <SDL_audio.h>
#include <stdint.h>

#include "../util/types.h"

#define AUDIO_MAX 4 // maximum number of sound effects that are allowed to play at once

struct audiodata {
	u8 const* buf; // pointer to the audio buffer
	u32 len;       // length in bytes of the audio buffer
	u32 ms;        // length in miliseconds of the audio buffer
};

enum audio_id {
	AUDIO_ID_MUSIC,
	AUDIO_ID_PLACE,

	// leave at end, will contain count
	AUDIO_ID_COUNT,
};

extern struct audiodata audio_dat[AUDIO_ID_COUNT];

/* loads the audio to be played, unless `AUDIO_MAX` has been reached, then this call will fail */
void audio_play(u32);

/* initialises the audio device, then loads the audio data */
int audio_init(int freq, SDL_AudioFormat fmt, u8 ch, u16 samples);

/* frees up resources held by the audio device and audio buffers */
void audio_free(void);
