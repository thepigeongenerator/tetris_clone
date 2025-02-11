#pragma once

#include <SDL_audio.h>
#include <stdint.h>

typedef struct {
    uint8_t* buf; // the audio buffer
    uint32_t len; // length in bytes of the audio buffer
    uint32_t sec; // length in seconds of the audio buffer
} audio_data;

typedef struct audio_player {
    struct audio_player* nxt; // pointer to the next audio fragment (can be NULL)
    uint8_t* buf;             // pointer to the current
    uint32_t len;             // length remaining of the audio buffer
} audio_player;

typedef struct {
    audio_player* audio_dat; // linked list of audio players
    SDL_AudioDeviceID id;    // the audio device id
    int32_t freq;
    SDL_AudioFormat fmt;
    uint8_t channels;
} audio_device;


audio_data audio_wav_load(audio_device const* audio_device, char const* file_path);
audio_device* audio_device_init(int freq, SDL_AudioFormat format, uint8_t channels, uint16_t samples);
void audio_play(audio_device* audio_device, audio_data const* audio);

void audio_device_free(audio_device* dev);
void audio_wav_unload(audio_data* dat);
