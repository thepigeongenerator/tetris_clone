#include "game.h"

#include <SDL_scancode.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../main.h"
#include "../window/audio.h"
#include "../window/colour8.h"
#include "./tetromino/shapes.h"
#include "SDL_audio.h"
#include "tetromino/placing.h"

// shuffle the array using a Fisher–Yates shuffle
static inline void shuffle(uint8_t const size, shape_id* const elmnts) {
    for (uint8_t i = 0; i < (size - 1); i++) {
        uint8_t const j = i + rand() % (size - i);
        shape_id const cache = elmnts[i];
        elmnts[i] = elmnts[j];
        elmnts[j] = cache;
    }
}

void next_shape(game_data* const dat) {
    dat->curr_idx++;                            // increase the current shape index
    dat->sel_x = COLUMNS / 2 - SHAPE_WIDTH / 2; // move the shape position to the centre
    dat->sel_y = 0;

    // return if know which shape is next
    if (dat->curr_idx < (TETROMINO_COUNT - 1))
        return;

    dat->curr_idx = 0;

    shuffle(TETROMINO_COUNT - 1, dat->nxt);
    shape_id cache = dat->nxt[0];
    dat->nxt[0] = dat->nxt[TETROMINO_COUNT - 1];
    dat->nxt[TETROMINO_COUNT - 1] = cache;
}

void game_init(game_data* const dat) {
    // zero-initialize the game data
    *dat = (game_data){0};

    // allocate size for each row
    for (int8_t i = 0; i < ROWS; i++) {
        dat->rows[i] = calloc(COLUMNS, sizeof(colour8));
        // game_data->rows[i][0] = (colour8){(uint8_t)((((i + 1) ^ ((i + 1) >> 3)) * 0x27) & 0xFF)}; // for debugging rows
    }

    // set a random seed using the system clock
    srand(time(NULL));

    // set the shape data in each slot to it's corrsponding ID
    for (shape_id i = 0; i < TETROMINO_COUNT; i++)
        dat->nxt[i] = i;

    dat->curr_idx = -1;                 // set the current index to the max so it becomes zero after increasement
    next_shape(dat);                    // select the next shape (shuffle should not be triggered)
    shuffle(TETROMINO_COUNT, dat->nxt); // manually trigger a shuffle

    // initialize audio
    dat->audio_device = audio_device_init(32000, AUDIO_S16, 1, 4096);
    dat->music = audio_wav_load(dat->audio_device, "korobeiniki.wav");
}

// called every time the game's state is updated
void game_update(game_data* const dat, uint8_t const* const keys) {
    if (keys[SDL_SCANCODE_ESCAPE])
        stop();

    time_t ctime = time(NULL);
    if (ctime > dat->music_timer) {
        dat->music_timer = ctime + dat->music.sec;
        audio_play(dat->audio_device, &dat->music);
    }

    InputData move = MOVE_NONE;
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) move |= MOVE_LEFT;
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) move |= MOVE_RIGHT;
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_SPACE]) move |= MOVE_DOWN;
    if (keys[SDL_SCANCODE_Q]) move |= MOVE_ROTLEFT;
    if (keys[SDL_SCANCODE_E]) move |= MOVE_ROTRIGHT;
    place_update(dat, move);
}

void game_free(game_data* const dat) {
    audio_wav_unload(&dat->music);
    audio_device_free(dat->audio_device);

    // clear each row
    for (int8_t i = 0; i < ROWS; i++) {
        free(dat->rows[i]);
        dat->rows[i] = NULL;
    }
}
