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
    AudioData const* const playing_audio;
    AudioDevice const audio_device;
} AudioCallbackData;

// audio callback from SDL_AudioSpec; called when the audio device needs more data
static void audio_mixer(void* const userdata, uint8_t* const stream, int32_t const len) {
    memset(stream, 0, len);                     // clear the playing audio
    AudioDevice const* const device = userdata; // get the callback data
    AudioData* const audio = device->playing_audio;

    for (int32_t i = 0; i < MAX_SOUNDS; i++) {
        // skip if the audio doesn't conain any further data
        if (audio[i].length <= 0) {
            continue;
        }

        // get the length of which we shall be mixing
        uint32_t const mix_length = SDL_min(audio[i].length, (uint32_t)len);

        // mix the audio with the stream
        SDL_MixAudioFormat(stream, audio[i].buffer, device->format, mix_length, SDL_MIX_MAXVOLUME);
        audio[i].buffer += mix_length; // move the pointer up a a bit
        audio[i].length -= mix_length; // move up the mixed amount
    }
}

// converts the audio to the format of the audio device
static void convert_audio(AudioDevice const* const audio_device, SDL_AudioSpec const wav_spec, uint8_t* const* const wav_buffer, uint32_t* const wav_length) {
    // build the audio converter with the audio given
    SDL_AudioCVT cvt = {0};
    SDL_BuildAudioCVT(&cvt, wav_spec.format, wav_spec.channels, wav_spec.freq, audio_device->format, audio_device->channels, audio_device->freq);

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
AudioData audio_load_wav(AudioDevice const* const audio_device, char const* const file_path) {
    SDL_AudioSpec wav_spec = {0};
    AudioData audio = {0};

    SDL_LoadWAV(file_path, &wav_spec, &audio.buffer, &audio.length);
    convert_audio(audio_device, wav_spec, &audio.buffer, &audio.length);
    audio.mixed_amount = audio.length;

    return audio;
}

// initializes the audio device
AudioDevice const* audio_device_init(int32_t const freq, SDL_AudioFormat const format, uint8_t const channels, Uint16 const samples) {
    // allocate memory for the audio device
    AudioDevice* const audio_device = malloc(sizeof(AudioDevice));

    // define the audio specification
    SDL_AudioSpec spec = {freq, format, channels, 0, samples, 0, 0, NULL, NULL};
    spec.callback = audio_mixer;
    spec.userdata = audio_device;

    // create the audio device
    *audio_device = (AudioDevice){
        SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0),
        freq,
        format,
        channels,
        calloc(MAX_SOUNDS, sizeof(AudioData)), // allocate memory on the heap for the playing audio array
    };

    // if the audio device isn't set, cause an error
    if (audio_device->id < 1) {
        error(ERROR_SDL_AUDIO_INIT, "AudioDivice failed to open! SDL Error: %s", SDL_GetError());
        return NULL;
    }

    // default state of the device is paused, so we unpause it here
    SDL_PauseAudioDevice(audio_device->id, 0);
    return audio_device;
}

// plays the audio
void audio_play(AudioDevice const* const audio_device, AudioData const audio) {
    AudioData* const playing_audio = audio_device->playing_audio;

    for (int32_t i = 0; i < MAX_SOUNDS; i++) {
        // overrite audio that has been deallocated
        if (playing_audio[i].length <= 0) {
            // override the audio
            playing_audio[i] = audio;
            break; // don't continue. :3
        }
    }
}
