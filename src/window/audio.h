#pragma once

#include <SDL_audio.h>
#include <stdint.h>

typedef struct {
    uint32_t len;
    uint32_t mix_amt;
    uint8_t* buf;
} audio_data;

typedef struct {
    audio_data* playing_audio;
    SDL_AudioDeviceID id;
    int32_t freq;
    SDL_AudioFormat fmt;
    uint8_t channels;
} audio_device;


audio_data audio_load_wav(audio_device const* audio_device, char const* file_path);
audio_device const* audio_device_init(int freq, SDL_AudioFormat format, uint8_t channels, uint16_t samples);
void audio_play(audio_device const* audio_device, audio_data const audio);
void audio_device_free(audio_device const* dev); // TODO: implement this
