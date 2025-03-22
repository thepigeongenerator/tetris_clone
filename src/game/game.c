#include "game.h"

#include <SDL_audio.h>
#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <SDL_timer.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../error.h"
#include "../window/audio.h"
#include "../window/colour/colour8.h"
#include "./tetromino/shapes.h"
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

void next_shape(gamedata* const dat) {
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

void game_init(gamedata* const dat, gametime* gt) {
    // set a random seed using the system clock
    srand(time(NULL));

    // initialize audio device
    audiodevice* ad = audio_device_init(32000, AUDIO_S16, 1, 4096);

    *dat = (gamedata){
        {0},                                   // row
        gt,                                    // time
        ad,                                    // audio_device
        audio_wav_load(ad, "korobeiniki.wav"), // music
        0,                                     // timer_music
        0,                                     // timer_update
        0,                                     // timer_input
        0,                                     // score
        {0},                                   // nxt
        0,                                     // curr_idx
        0,                                     // sel_x
        0,                                     // sel_y
        true,                                  // run
    };

    // initialize the rows within the game data
    for (int8_t i = 0; i < ROWS; i++) {
        dat->rows[i] = calloc(COLUMNS, sizeof(colour8));

        if (dat->rows[i] == NULL)
            fatal(ERROR_STD_MEMORY_INIT, __FILE_NAME__, __LINE__, "something went wrong when allocating memory for row %i", i);
    }

    // set the shape data in each slot to it's corrsponding ID
    for (shape_id i = 0; i < TETROMINO_COUNT; i++)
        dat->nxt[i] = i;

    dat->curr_idx = -1;                 // set the current index to the max so it becomes zero after increasement
    next_shape(dat);                    // select the next shape (shuffle should not be triggered)
    shuffle(TETROMINO_COUNT, dat->nxt); // manually trigger a shuffle
}

// called every time the game's state is updated
void game_update(gamedata* const dat) {
    uint8_t const* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_ESCAPE])
        dat->run = false;

    input_data move = MOVE_NONE; // contains the move data
    uint32_t ctime = SDL_GetTicks();

    if (ctime > dat->timer_update) {
        dat->timer_update = ctime + 500;
        move |= MOVE_DOWN;
    }

    if (ctime > dat->timer_music) {
        dat->timer_music = ctime + (dat->music.ms);
        audio_play(dat->audio_device, &dat->music);
    }

    if (ctime > dat->timer_input) {
        input_data umove = MOVE_NONE;

        // get the input data and apply it to move
        if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) umove |= MOVE_LEFT;
        if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) umove |= MOVE_RIGHT;
        if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_SPACE]) umove |= MOVE_DOWN;
        if (keys[SDL_SCANCODE_Q]) umove |= MOVE_ROTLEFT;
        if (keys[SDL_SCANCODE_E]) umove |= MOVE_ROTRIGHT;

        if (umove != MOVE_NONE) {
            dat->timer_input = ctime + 60;
            move |= umove;
        }
    }

    // update the block position
    if (move != MOVE_NONE)
        place_update(dat, move);
}

void game_free(gamedata* const dat) {
    audio_wav_unload(&dat->music);
    audio_device_free(dat->audio_device);

    // clear each row
    for (int8_t i = 0; i < ROWS; i++) {
        free(dat->rows[i]);
        dat->rows[i] = NULL;
    }

    // zero-out the rest of the data
    *dat = (gamedata){0};
}
