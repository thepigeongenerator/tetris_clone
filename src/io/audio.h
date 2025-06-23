#pragma once

#include <SDL_audio.h>
#include <stdint.h>

#define AUDIO_MAX 4 // maximum number of sound effects that are allowed to play at once

struct audiodata {
	uint8_t* buf; // pointer to the audio buffer
	uint32_t len; // length in bytes of the audio buffer
	uint32_t ms;  // length in miliseconds of the audio buffer
};
typedef struct audiodata audiodata;

int audio_device_init(int, SDL_AudioFormat, uint8_t, uint16_t);
void audio_device_free(void);
void audio_play(audiodata const*);

audiodata audio_wav_load(char const*);
void audio_wav_unload(audiodata*);
