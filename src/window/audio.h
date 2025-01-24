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


AudioData audio_load_wav(const AudioDevice* audio_device, const char* file_path);
AudioDevice* audio_device_init(const int freq, const SDL_AudioFormat format, const uint8_t channels, const uint16_t samples);
void audio_play(const AudioDevice* audio_device, const AudioData audio);
