#pragma once
#include <SDL2/SDL.h>

typedef struct {
    Uint32 length;
    Uint32 mixed_amount;
    Uint8* buffer;
} AudioData;

typedef struct {
    SDL_AudioDeviceID id;
    int freq;
    SDL_AudioFormat format;
    Uint8 channels;
    AudioData* playing_audio;
} AudioDevice;


AudioData audio_load_wav(const AudioDevice* audio_device, const char* file_path);
AudioDevice* audio_device_init(const int freq, const SDL_AudioFormat format, const Uint8 channels, const Uint16 samples);
void audio_play(const AudioDevice* audio_device, const AudioData audio);
