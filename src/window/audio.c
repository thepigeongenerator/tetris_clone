#include "audio.h"

#include <SDL_audio.h>
#include <SDL_error.h>
#include <SDL_stdinc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../errors.h"

//
// audio mixing
//
// audio callback from SDL_AudioSpec; called when the audio device needs more data
static void audio_mixer(void* const userdata, uint8_t* const stream, int32_t const len) {
    memset(stream, 0, len);             // clear the playing audio
    audio_device* const dev = userdata; // get the callback data

    audio_player* prev = NULL;
    audio_player* curr = dev->audio_dat;
    while (curr != NULL) {
        // if the current length, remove self from the list
        if (curr->len == 0) {
            audio_player* ncurr = curr->nxt; // store the next player as the new current player

            // set the current player to the new current player & free the current player
            free(curr);
            curr = ncurr; // ncurr can be NULL, this is fine

            // update the previous pointer accordingly
            if (prev == NULL)
                dev->audio_dat = ncurr;
            else
                prev->nxt = ncurr;

            // continue code execution
            continue;
        }

        // get the length of which we shall be mixing
        uint32_t const mix_length = SDL_min(curr->len, (uint32_t)len);

        // mix the audio with the stream
        SDL_MixAudioFormat(stream, curr->buf, dev->fmt, mix_length, SDL_MIX_MAXVOLUME);
        curr->buf += mix_length; // move the pointer up a a bit
        curr->len -= mix_length; // move up the mixed amount

        // increment across the list
        prev = curr;
        curr = curr->nxt;
    }
}

// converts the audio to the format of the audio device, reallocates wav_buf to a new size outputted to wav_len
static void convert_audio(audio_device const* const dev, SDL_AudioSpec const wav_spec, uint8_t** const wav_buf, uint32_t* const wav_len) {
    // build the audio converter with the audio given
    SDL_AudioCVT cvt = {0};
    SDL_BuildAudioCVT(&cvt, wav_spec.format, wav_spec.channels, wav_spec.freq, dev->fmt, dev->channels, dev->freq);
    cvt.len = (*wav_len) * wav_spec.channels;            // calculate the size of the source data in bytes by multiplying the length by the amount of channels (warn: uint32_t -> int32_t)
    cvt.buf = realloc(*wav_buf, cvt.len * cvt.len_mult); // grow the inputted buffer for the conversion

    // performs the conversion
    if (SDL_ConvertAudio(&cvt) != 0)
        error(ERROR_SDL_AUDIO_INIT, "something went wrong when converting an audio buffer! SDL Error: %s", SDL_GetError());

    // set the length to the new length
    *wav_len = cvt.len_cvt;

    // reallocate the conversion buffer to match the new size
    *wav_buf = realloc(cvt.buf, cvt.len_cvt);
    if (*wav_buf == NULL)
        error(ERROR_MISC, "null value when reallocating the audio buffer");
}


//
// audio / audio device management
//
// loads a WAV file and returns the relevant information
audio_data audio_wav_load(audio_device const* const dev, char const* const fpath) {
    SDL_AudioSpec wav_spec = {0};
    audio_data audio = {0};

    SDL_LoadWAV(fpath, &wav_spec, &audio.buf, &audio.len);
    convert_audio(dev, wav_spec, &audio.buf, &audio.len);

    // calculate the amount of seconds that the audio fragment has
    audio.ms = 1000 * (((audio.len) / (SDL_AUDIO_BITSIZE(dev->fmt) / 8)) / wav_spec.channels / dev->freq);

    return audio;
}

// initializes the audio device
audio_device* audio_device_init(int32_t const freq, SDL_AudioFormat const fmt, uint8_t const channels, uint16_t const samples) {
    // allocate memory for the audio device
    audio_device* const dev = malloc(sizeof(audio_device));

    // define the audio specification
    SDL_AudioSpec spec = {freq, fmt, channels, 0, samples, 0, 0, NULL, NULL};
    spec.callback = audio_mixer;
    spec.userdata = dev;

    // create the audio device
    *dev = (audio_device){
        NULL, // allocate memory on the heap for the playing audio array
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
void audio_play(audio_device* const dev, audio_data const* audio) {
    // create an audio player
    audio_player* player = malloc(sizeof(audio_player));
    *player = (audio_player){
        dev->audio_dat, // set nxt to the first item in dev (can be NULL, this is fine)
        audio->buf,
        audio->len,
    };

    // assign ourselves to the first item
    dev->audio_dat = player;
}

// frees the audio device
void audio_device_free(audio_device* const dev) {
    SDL_CloseAudioDevice(dev->id);
    free(dev->audio_dat);
    free(dev);
}

// frees the buffer of the audio data
void audio_wav_unload(audio_data* dat) {
    free(dat->buf);
}
