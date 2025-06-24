#include "game.h"

#include <SDL_audio.h>
#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "../io/audio.h"
#include "../util/types.h"
#include "./tetromino/shapes.h"
#include "gametime.h"
#include "tetromino/placing.h"

/* shuffle the array using a Fisher–Yates shuffle */
static inline void shuffle(uint8_t const size, u8* const elmnts) {
	for (uint i = 0; i < (size - 1); i++) {
		uint const j = i + rand() % (size - i);
		u8 const cache = elmnts[i];
		elmnts[i] = elmnts[j];
		elmnts[j] = cache;
	}
}

void next_shape(gamedata* const dat) {
	dat->curr_idx++;                            // increase the current shape index
	dat->sel_x = COLUMNS / 2 - SHAPE_WIDTH / 2; // move the shape position to the centre
	dat->sel_y = 0;

	// return if know which shape is next
	if (dat->curr_idx < (TETROMINO_COUNT - 1))
		return;

	dat->curr_idx = 0;

	shuffle(TETROMINO_COUNT - 1, dat->nxt);
	u8 cache = dat->nxt[0];
	dat->nxt[0] = dat->nxt[TETROMINO_COUNT - 1];
	dat->nxt[TETROMINO_COUNT - 1] = cache;
}

void game_init(gamedata* const dat) {
	// set a random seed using the system clock
	srand(time(NULL));

	struct gametime gt = {{0}, 0};
	gametime_get(&gt.ts);

	// initialize audio device
	audio_device_init(32000, AUDIO_S16, 1, 4096);

	*dat = (gamedata){
		{0},                               // rowdat
		{0},                               // row
		gt,                                // time
		audio_wav_load("korobeiniki.wav"), // music
		audio_wav_load("place.wav"),       // place_sfx
		0,                                 // score
		{0},                               // nxt
		0,                                 // curr_idx
		0,                                 // sel_x
		0,                                 // sel_y
		true,                              // run
	};

	// initialize the rows within the game data
	for (int8_t i = 0; i < ROWS; i++) {
		dat->rows[i] = dat->rowdat + (i * COLUMNS);
	}

	// set the shape data in each slot to it's corrsponding ID
	for (uint i = 0; i < TETROMINO_COUNT; i++)
		dat->nxt[i] = i;

	dat->curr_idx = -1;                 // set the current index to the max so it becomes zero after increasement
	next_shape(dat);                    // select the next shape (shuffle should not be triggered)
	shuffle(TETROMINO_COUNT, dat->nxt); // manually trigger a shuffle
}

// updates the gametime
static inline void update_gametime(gamedata* dat) {
	struct timespec ts;
	gametime_get(&ts);
	dat->time.ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
	dat->time.ts = ts;
}

// called every time the game's state is updated
void game_update(gamedata* const dat) {
	static time_t timer_update = 0;
	static time_t timer_music = 0;
	static time_t timer_move = 0;
	static time_t timer_rot = 0;
	update_gametime(dat);
	uint8_t const* keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_ESCAPE])
		dat->run = false;

	input_data move = MOVE_NONE; // contains the move data
	time_t ctime = dat->time.ms;

	if (ctime > timer_update) {
		timer_update = ctime + 500;
		move |= MOVE_DOWN;
	}

	if (ctime > timer_music) {
		timer_music = ctime + (dat->music.ms);
		audio_play(&dat->music);
	}

	// for rotation updating
	if (ctime > timer_rot) {
		input_data urot = MOVE_NONE;
		if (keys[SDL_SCANCODE_Q]) urot |= MOVE_ROTLEFT;
		if (keys[SDL_SCANCODE_E]) urot |= MOVE_ROTRIGHT;

		if (urot != MOVE_NONE) {
			timer_rot = ctime + 100;
			move |= urot;
		}
	}

	// for movement updating
	if (ctime > timer_move) {
		input_data umove = MOVE_NONE;
		if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) umove |= MOVE_LEFT;
		if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) umove |= MOVE_RIGHT;
		if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_SPACE]) umove |= MOVE_DOWN;

		if (umove != MOVE_NONE) {
			timer_move = ctime + 20;
			move |= umove;
		}
	}

	// update the block position
	if (move != MOVE_NONE)
		place_update(dat, move);
}

void game_free(gamedata* const dat) {
	audio_wav_unload(&dat->music);
	audio_wav_unload(&dat->place_sfx);
	audio_device_free();

	// zero-out the rest of the data
	*dat = (gamedata){0};
}
