#pragma once

#include <SDL_audio.h>
#include <stdint.h>

struct audiodata {
    uint8_t* buf; // pointer to the audio buffer
    uint32_t len; // length in bytes of the audio buffer
    uint32_t ms;  // length in miliseconds of the audio buffer
};

// contains the data of the audio fragments to be played
struct audioplayer {
    struct audioplayer* nxt; // pointer to the next audioplayer (may be null)
    uint8_t* buf;            // pointer to the current item in the buffer to be played
    uint32_t len;            // the length in bytes that the buffer has remaining
};

struct audiodevice {
    struct audioplayer* audio_players;
    SDL_AudioDeviceID id;
    int freq;
    SDL_AudioFormat fmt;
    uint8_t channels;
};

typedef struct audiodata audiodata;
typedef struct audiodevice audiodevice;

audiodevice* audio_device_init(int, SDL_AudioFormat, uint8_t, uint16_t);
void audio_play(audiodevice*, audiodata const*);
void audio_device_free(audiodevice*);

audiodata audio_wav_load(audiodevice const*, char const*);
void audio_wav_unload(audiodata*);
