#include "audio.h"

#include <SDL_audio.h>
#include <SDL_error.h>
#include <SDL_stdinc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../error.h"
#include "../util/compat.h"

static void audiomixer(void* const userdata, uint8_t* const stream, int const len) {
    memset(stream, 0, len);            // clear the playing audio
    audiodevice* const dev = userdata; // retreive the callback data

    // return if dev is null, since it can fail to initialize
    if (dev == NULL) return;

    struct audioplayer* prev = NULL;
    struct audioplayer* curr = dev->audio_players;
    while (curr != NULL) {
        // if the current audio fragment has reached the end of their data
        if (curr->len == 0) {
            struct audioplayer* ncurr = curr->nxt;

            // free the memory allocated to it and assign the next to to the currently playing
            free(curr);
            curr = ncurr;

            // write to the audio device if prev hasn't been set yet
            if (prev == NULL)
                dev->audio_players = curr;
            else
                prev->nxt = curr;

            // continue so if curr is now NULL, the loop stops
            continue;
        }

        // calculate how much of the current audio player we should mix into the stream
        int const mixlen = SDL_min(curr->len, (unsigned)len);

        // mix the current buffer into the stream, and update the audio player values accordingly
        SDL_MixAudioFormat(stream, curr->buf, dev->fmt, mixlen, SDL_MIX_MAXVOLUME);
        curr->buf += mixlen;
        curr->len -= mixlen;

        // increment the current node
        prev = curr;
        curr = curr->nxt;
    }
}

// converts the inputted audio to the format of dev
// returns 1 upon failure, 0 upon success. When 1 is returned *bufptr will be freed. Otherwise *bufptr is reallocated
static int8_t audio_cvt(audiodevice const* dev, SDL_AudioSpec const* spec, uint8_t** bufptr, unsigned* len) {
    // init the converter
    SDL_AudioCVT cvt;
    if (SDL_BuildAudioCVT(&cvt, spec->format, spec->channels, spec->freq, dev->fmt, dev->channels, dev->freq) < 0) {
        error("%s:%u could not build the audio converter! SDL Error: %s", __FILE_NAME__, __LINE__, SDL_GetError());
        free(*bufptr); // free the buffer upon an error, as we won't be using this
        return 1;
    } else if (!cvt.needed) { // ensure the conversion is necessary
        return 0;
    }
    cvt.len = (*len);                                   // specify the length of the source data buffer in bytes (warn: uint32_t -> int32_t)
    cvt.buf = realloc(*bufptr, cvt.len * cvt.len_mult); // grow the inputted buffer for the conversion

    // ensure the conversion buffer reallocation goes correctly
    if (cvt.buf == NULL) {
        error("%s:%u failed to reallocate the audio buffer to the new size of %u bytes!", __FILE_NAME__, __LINE__, cvt.len);
        free(*bufptr); // free the inputted pointer, as realloc doesn't clear this address if it fails
        return 1;
    }

    // converts the audio to the new format
    if (SDL_ConvertAudio(&cvt)) {
        error("%s:%u something went wrong when loading/converting an audio buffer! SDL Error: %s", __FILE_NAME__, __LINE__, SDL_GetError());
        free(cvt.buf); // free the conversion buffer if it fails, as realloc moved the data to this adress; old adress is no longer valid
        return 1;
    }

    // update output
    *len = cvt.len_cvt;                      // set the length to the new length after the conversion
    *bufptr = realloc(cvt.buf, cvt.len_cvt); // reallocate the buffer to the new size
    if (*bufptr == NULL) {
        warn("%s:%u something went wrong whilst shrinking the audio buffer whilst converting!", __FILE_NAME__, __LINE__);
        *bufptr = cvt.buf; // use the conversion buffer, as this one will be valid if realloc fails
    }

    return 0;
}

audiodevice* audio_device_init(int freq, SDL_AudioFormat fmt, uint8_t channels, uint16_t samples) {
    audiodevice* dev = malloc(sizeof(audiodevice));

    if (dev == NULL) {
        error("%s:%u null pointer when allocating memory for the audio device!", __FILE_NAME__, __LINE__);
        return NULL;
    }

    // define the audio specification
    SDL_AudioSpec spec = {freq, fmt, channels, 0, samples, 0, 0, NULL, NULL};
    spec.callback = audiomixer;
    spec.userdata = dev;

    // create the audio device
    *dev = (audiodevice){
        NULL,
        SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0),
        freq,
        fmt,
        channels,
    };

    if (dev->id < 1) {
        error("%s:%u audio device failed to open! SDL Error: %s", __FILE_NAME__, __LINE__, SDL_GetError());
        free(dev);
        return NULL;
    }

    // default state of the device is paused, so we unpause it here
    SDL_PauseAudioDevice(dev->id, 0);
    return dev;
}

void audio_play(audiodevice* dev, audiodata const* audio) {
    if (dev == NULL) return;     // dev might fail to initialize
    if (audio->len == 0) return; // audio might fail to initialize

    // create an audio player
    struct audioplayer* player = malloc(sizeof(struct audioplayer));
    *player = (struct audioplayer){
        dev->audio_players, // set nxt to the first item in dev (can be NULL, this is fine)
        audio->buf,
        audio->len,
    };

    // assign ourselves to the first item
    dev->audio_players = player;
}

void audio_device_free(audiodevice* dev) {
    if (dev == NULL) return;
    SDL_CloseAudioDevice(dev->id);

    struct audioplayer* curr = dev->audio_players;

    // free all audio players
    while (curr != NULL) {
        dev->audio_players = curr->nxt; // use audio_players in dev as a cache
        free(curr);
        curr = dev->audio_players;
    }

    // free the audio device itself
    free(dev);
}

audiodata audio_wav_load(audiodevice const* dev, char const* fpath) {
    if (dev == NULL) return (audiodata){0};
    SDL_AudioSpec spec;
    audiodata audio;

    debug("loading audio file '%s'...", fpath);

    if (!faccess(fpath, FA_R)) {
        error("%s:%u audio file either isn't readable or doesn't exist. path: '%s'!", __FILE_NAME__, __LINE__, fpath);
        return (audiodata){0};
    }

    // load and parse the audio to the correct format
    SDL_LoadWAV(fpath, &spec, &audio.buf, &audio.len);
    if (audio_cvt(dev, &spec, &audio.buf, &audio.len)) {
        return (audiodata){0};
    }

    // calculate the time in milliseconds of the audio fragment
    // by dividing the audio bytelength by the format's bitsize, by the audio device's channels and the audio device's frequency
    audio.ms = (((1000 * audio.len) / (SDL_AUDIO_BITSIZE(dev->fmt) / 8)) / dev->channels / dev->freq);

    return audio;
}

void audio_wav_unload(audiodata* audio) {
    free(audio->buf);
    *audio = (audiodata){0}; // zero out all audio data
}
