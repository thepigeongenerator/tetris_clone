#include "audio.h"

#include <SDL_audio.h>
#include <SDL_error.h>
#include <SDL_stdinc.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../error.h"
#include "../util/compat.h"

struct audioplayer {
	uint8_t const* buf;
	int len;
};

struct audiodevice {
	struct audioplayer audio[AUDIO_MAX];
	unsigned id;
	int freq;
	SDL_AudioFormat fmt;
	uint8_t ch; // audio channels
} dev;

/* mixes the audio output stream, using the different audio as sources */
static void audiomixer(void* const userdata, uint8_t* const stream, int const len) {
	(void)userdata;
	memset(stream, 0, len); // clear the playing audio

	// update the counts for the audio array
	for (unsigned i = 0; i < AUDIO_MAX; i++) {
		if (dev.audio[i].len > 0) {
			unsigned mixlen = SDL_min(len, dev.audio[i].len);
			SDL_MixAudioFormat(stream, dev.audio[i].buf, dev.fmt, mixlen, SDL_MIX_MAXVOLUME);
			dev.audio[i].len -= mixlen;
			dev.audio[i].buf += mixlen;
		}
	}
}

/* converts input audio buffer to one that matches the buffer of dev.
 * `len` is a pointer to the current size, the new size will be written to this location.
 * returns the pointer to the audio buffer to use, or NULL, when something went wrong.
 * NULL will never be returned after the conversion */
static uint8_t* audio_cvt(SDL_AudioSpec const* spec, uint8_t* bufptr, unsigned* len) {
	if (!bufptr) return NULL;

	// init the converter
	SDL_AudioCVT cvt;
	if (SDL_BuildAudioCVT(&cvt, spec->format, spec->channels, spec->freq, dev.fmt, dev.ch, dev.freq) < 0) {
		error("could not build the audio converter! SDL Error: %s", SDL_GetError());
		return NULL;
	}
	if (!cvt.needed) return bufptr; // ensure the conversion is necessary

	// grow the inputted buffer for the conversion
	cvt.len = *len;
	cvt.buf = realloc(bufptr, cvt.len * cvt.len_mult);
	if (!cvt.buf) {
		warn("failed to grow the audio buffer to the new size of %u bytes! audio may be bugged", cvt.len);
		return bufptr;
	}

	// convert the audio to the new format
	if (SDL_ConvertAudio(&cvt) < 0) {
		warn("couldn't convert an audio buffer to the set format, audio may be bugged! SDL Error: %s", SDL_GetError());
		assert(cvt.buf);
		return cvt.buf;
	}
	assert(cvt.buf);

	// shrink to reduce memory footprint
	*len = cvt.len_cvt;
	bufptr = realloc(cvt.buf, *len);
	return bufptr ? bufptr : cvt.buf;
}

int audio_device_init(int freq, SDL_AudioFormat fmt, uint8_t channels, uint16_t samples) {
	// define the audio specification
	SDL_AudioSpec spec = {freq, fmt, channels, 0, samples, 0, 0, NULL, NULL};
	spec.callback = audiomixer;
	spec.userdata = NULL;

	unsigned id = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);

	// create the audio device
	dev = (struct audiodevice){
		{0},
		id,
		freq,
		fmt,
		channels,
	};

	if (id) {
		// default state of the device is paused, so we unpause it here
		SDL_PauseAudioDevice(id, 0);
		return 0;
	} else return 1;
}

void audio_device_free(void) {
	SDL_CloseAudioDevice(dev.id);
	dev = (struct audiodevice){0};
}

void audio_play(audiodata const* audio) {
	if (!dev.id) return;
	if (!audio->buf) return;

	unsigned i = 0;
	while (i < AUDIO_MAX && dev.audio[i].len > 0) i++;
	if (i < AUDIO_MAX)
		dev.audio[i] = (struct audioplayer){audio->buf, audio->len};
}

audiodata audio_wav_load(char const* fpath) {
	debug("loading audio file '%s'...", fpath);

	if (faccess(fpath, FA_R)) {
		error("%s:%u audio file either isn't readable or doesn't exist. path: '%s'!", __FILE_NAME__, __LINE__, fpath);
		return (struct audiodata){0};
	}

	// load the audio
	SDL_AudioSpec spec;
	audiodata audio;
	SDL_LoadWAV(fpath, &spec, &audio.buf, &audio.len);
	if (!audio.buf) return audio;

	// convert the audio data to the format reflecting dev
	void* ptr = audio_cvt(&spec, audio.buf, &audio.len);
	if (!ptr) free(audio.buf); // free the buffer if NULL was returned; failure
	audio.buf = ptr;

	// calculate the time in milliseconds of the audio fragment
	// by dividing the audio bytelength by the format's bitsize, by the audio device's channels and the audio device's frequency
	audio.ms = (((1000 * audio.len) / (SDL_AUDIO_BITSIZE(dev.fmt) / 8)) / dev.ch / dev.freq);

	return audio;
}

void audio_wav_unload(audiodata* audio) {
	free(audio->buf);
	*audio = (audiodata){0}; // zero out all audio data
}
