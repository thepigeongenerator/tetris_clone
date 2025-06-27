// TODO: audio stores audio buffers at indicies that the user specifies. (array growth is the next power of two from the idx)
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
#include "../util/types.h"

struct audioplayer {
	u8 const* buf;
	int len;
};

static struct audiodevice {
	struct audioplayer audio[AUDIO_MAX];
	u32 id;
	SDL_AudioSpec spec;
} dev;

struct audiodata audio_dat[AUDIO_ID_COUNT] = {0}; // contains pointers to audio buffers.

static char const* const audio_path[AUDIO_ID_COUNT] = {
	"korobeiniki.wav",
	"place.wav",
};

/* mixes the audio output stream, using the different audio as sources */
static void audiomixer(void* const userdata, u8* const stream, int const len) {
	(void)userdata;
	memset(stream, 0, len); // clear the playing audio

	// update the counts for the audio array
	for (unsigned i = 0; i < AUDIO_MAX; i++) {
		if (dev.audio[i].len > 0) {
			unsigned mixlen = SDL_min(len, dev.audio[i].len);
			SDL_MixAudioFormat(stream, dev.audio[i].buf, dev.spec.format, mixlen, SDL_MIX_MAXVOLUME);
			dev.audio[i].len -= mixlen;
			dev.audio[i].buf += mixlen;
		}
	}
}

/* converts input audio buffer to one that matches the buffer of dev.
 * `len` is a pointer to the current size, the new size will be written to this location.
 * returns the pointer to the audio buffer to use, or NULL, when something went wrong.
 * NULL will never be returned after the conversion */
static u8* audio_cvt(SDL_AudioSpec const* spec, u8* bufptr, unsigned* len) {
	if (!bufptr) return NULL;

	// init the converter
	SDL_AudioCVT cvt;
	if (SDL_BuildAudioCVT(&cvt, spec->format, spec->channels, spec->freq, dev.spec.format, dev.spec.channels, dev.spec.freq) < 0) {
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

/* computes the time in milliseconds of the audio fragment by dividing
 * the audio byte length by the format's bit size, which is divided by
 * the audio device's channels and frequency */
static inline u32 audio_btoms(u32 len) {
	return (((1000 * len) / (SDL_AUDIO_BITSIZE(dev.spec.format) / 8)) / dev.spec.channels / dev.spec.freq);
}

/* loads a `struct audiodata` from `fpat` to `out`. */
static void audio_wav_load(char const* restrict fpat, struct audiodata* restrict out) {
	debug("loading audio file '%s'...", fpat);
	if (faccess(fpat, FA_R)) {
		error("audio file either isn't readable or doesn't exist. path: '%s'!", fpat);
		return;
	}

	// load the audio
	u32 len;
	u8 *ptr, *tmp;
	SDL_AudioSpec spec;
	SDL_LoadWAV(fpat, &spec, &ptr, &len);

	// convert the audio data to the format reflecting dev
	tmp = audio_cvt(&spec, ptr, &len);
	if (!tmp) free(ptr); // free the buffer if NULL was returned; failure

	*out = (struct audiodata){tmp, len, audio_btoms(len)};
}

/* loads the audio data into the buffer */
static inline void audio_load(void) {
	for (size_t i = 0; i < AUDIO_ID_COUNT; i++)
		audio_wav_load(audio_path[i], &audio_dat[i]);
}

void audio_play(u32 audio_id) {
	if (!dev.id) return;
	size_t i = 0;
	while (i < AUDIO_MAX && dev.audio[i].len > 0) i++;
	if (i >= AUDIO_MAX) return;

	dev.audio[i] = (struct audioplayer){
		audio_dat[audio_id].buf,
		audio_dat[audio_id].len,
	};
}

int audio_init(int freq, SDL_AudioFormat fmt, u8 ch, u16 samples) {
	// initialise the audio device + specification
	SDL_AudioSpec spec = {freq, fmt, ch, 0, samples, 0, 0, audiomixer, NULL};
	unsigned id = SDL_OpenAudioDevice(NULL, 0, &spec, &spec, 0);
	dev = (struct audiodevice){{0}, id, spec};

	if (!id) return 1;
	SDL_PauseAudioDevice(id, 0);
	audio_load();
	return 0;
}

void audio_free(void) {
	SDL_CloseAudioDevice(dev.id);
	dev = (struct audiodevice){0};

	for (size_t i = 0; i < AUDIO_ID_COUNT; i++)
		free((void*)audio_dat[i].buf);
}
