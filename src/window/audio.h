#pragma once

#include <SDL_audio.h>
#include <stdint.h>

typedef struct {
    uint32_t length;
    uint32_t mixed_amount;
    uint8_t* buffer;
} AudioData;

typedef struct {
    SDL_AudioDeviceID id;
    int32_t freq;
    SDL_AudioFormat format;
    uint8_t channels;
    AudioData* playing_audio;
} AudioDevice;


AudioData audio_load_wav(AudioDevice const* audio_device, char const* file_path);
AudioDevice const* audio_device_init(int freq, SDL_AudioFormat format, uint8_t channels, uint16_t samples);
void audio_play(AudioDevice const* audio_device, AudioData const audio);
