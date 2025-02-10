#include "audio.h"

#include <SDL_audio.h>
#include <SDL_error.h>
#include <SDL_stdinc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../errors.h"

// the maximum amount of sounds that can play at once
#define MAX_SOUNDS 64

typedef struct {
    audio_data const* const playing_audio;
    audio_device const audio_device;
} AudioCallbackData;

// audio callback from SDL_AudioSpec; called when the audio device needs more data
static void audio_mixer(void* const userdata, uint8_t* const stream, int32_t const len) {
    memset(stream, 0, len);                      // clear the playing audio
    audio_device const* const device = userdata; // get the callback data
    audio_data* const audio = device->playing_audio;

    for (int32_t i = 0; i < MAX_SOUNDS; i++) {
        // skip if the audio doesn't conain any further data
        if (audio[i].len <= 0) {
            continue;
        }

        // get the length of which we shall be mixing
        uint32_t const mix_length = SDL_min(audio[i].len, (uint32_t)len);

        // mix the audio with the stream
        SDL_MixAudioFormat(stream, audio[i].buf, device->fmt, mix_length, SDL_MIX_MAXVOLUME);
        audio[i].buf += mix_length; // move the pointer up a a bit
        audio[i].len -= mix_length; // move up the mixed amount
    }
}

// converts the audio to the format of the audio device
static void convert_audio(audio_device const* const audio_device, SDL_AudioSpec const wav_spec, uint8_t* const* const wav_buffer, uint32_t* const wav_length) {
    // build the audio converter with the audio given
    SDL_AudioCVT cvt = {0};
    SDL_BuildAudioCVT(&cvt, wav_spec.format, wav_spec.channels, wav_spec.freq, audio_device->fmt, audio_device->channels, audio_device->freq);

    // suddenly becomes signed
    cvt.len = (*wav_length) * wav_spec.channels;            // the buffer length
    cvt.buf = (uint8_t*)SDL_malloc(cvt.len * cvt.len_mult); // allocate size for the new buffer
    memcpy(cvt.buf, *wav_buffer, *wav_length);              // copy wav data to cvt buffer;

    // convert
    SDL_ConvertAudio(&cvt);

    // output
    *wav_length = cvt.len_cvt;                 // set the length to the new length
    memcpy(*wav_buffer, cvt.buf, cvt.len_cvt); // copy converted cvt buffer back to wav buffer

    free(cvt.buf); // free the memory allocated to the cvt buffer
}

// loads a WAV file and returns the relevant information
audio_data audio_load_wav(audio_device const* const dev, char const* const fpath) {
    SDL_AudioSpec wav_spec = {0};
    audio_data audio = {0};

    SDL_LoadWAV(fpath, &wav_spec, &audio.buf, &audio.len);
    convert_audio(dev, wav_spec, &audio.buf, &audio.len);
    audio.mix_amt = audio.len;

    return audio;
}

// initializes the audio device
audio_device const* audio_device_init(int32_t const freq, SDL_AudioFormat const fmt, uint8_t const channels, uint16_t const samples) {
    // allocate memory for the audio device
    audio_device* const dev = malloc(sizeof(audio_device));

    // define the audio specification
    SDL_AudioSpec spec = {freq, fmt, channels, 0, samples, 0, 0, NULL, NULL};
    spec.callback = audio_mixer;
    spec.userdata = dev;

    // create the audio device
    *dev = (audio_device){
        calloc(MAX_SOUNDS, sizeof(audio_data)), // allocate memory on the heap for the playing audio array
        SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0),
        freq,
        fmt,
        channels,
    };

    // if the audio device isn't set, cause an error
    if (dev->id < 1) {
        error(ERROR_SDL_AUDIO_INIT, "AudioDivice failed to open! SDL Error: %s", SDL_GetError());
        return NULL;
    }

    // default state of the device is paused, so we unpause it here
    SDL_PauseAudioDevice(dev->id, 0);
    return dev;
}

// plays the audio
void audio_play(audio_device const* const dev, audio_data const audio) {
    audio_data* const playing_audio = dev->playing_audio;

    for (int32_t i = 0; i < MAX_SOUNDS; i++) {
        // overrite audio that has been deallocated
        if (playing_audio[i].len <= 0) {
            // override the audio
            playing_audio[i] = audio;
            break; // don't continue. :3
        }
    }
}
