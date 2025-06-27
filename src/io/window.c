#include "window.h"

#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_error.h>
#include <SDL_video.h>
#include <assert.h>
#include <time.h>

#include "../error.h"
#include "../game/game.h"
#include "../game/time.h"
#include "SDL_ttf.h"
#include "audio.h"
#include "input.h"
#include "render.h"

static SDL_Window* win = NULL;
static bool close = false;

void window_init(struct gamedata const* gdat) {
	assert(!win && !close);
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		fatal(ERROR_SDL_INIT, "SDL could not initialize! SDL Error: %s", SDL_GetError());
	if (TTF_Init() < 0)
		fatal(ERROR_SDL_INIT, "TTF could not initialize! TTF Error: %s", TTF_GetError());

	win = SDL_CreateWindow("tetris clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == NULL)
		fatal(ERROR_SDL_RENDERING_INIT, "Window failed to be created! SDL Error: %s", SDL_GetError());

	render_init(win, gdat);
	audio_init(32000, AUDIO_S16, 1, 4096);
}

void window_free(void) {
	assert(win);
	render_free();
	SDL_DestroyWindow(win);
	win = NULL;
	close = false;

	audio_free();
}

void window_open(void) {
	while (!close) {
		size_t time = time_pull();
		game_update(input_getdat(), time);
		render_update();

		static time_t timeout = 0;
		if (time_poll(time, audio_dat[AUDIO_ID_MUSIC].ms, &timeout))
			audio_play(AUDIO_ID_MUSIC);
	}
}

void window_close(void) {
	close = true;
}
