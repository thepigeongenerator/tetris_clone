#include "game.h"

#include <SDL_scancode.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../main.h"
#include "../window/colour8.h"
#include "./tetromino/shapes.h"
#include "tetromino/placing.h"


// shuffle the array using a Fisher–Yates shuffle
static inline void shuffle(uint8_t const size, ShapeId* const elmnts) {
    for (uint8_t i = 0; i < (size - 1); i++) {
        uint8_t const j = i + rand() % (size - i);
        ShapeId const cache = elmnts[i];
        elmnts[i] = elmnts[j];
        elmnts[j] = cache;
    }
}

void next_shape(GameData* const game_data) {
    game_data->curr_idx++;                            // increase the current shape index
    game_data->sel_x = COLUMNS / 2 - SHAPE_WIDTH / 2; // move the shape position to the centre
    game_data->sel_y = 0;

    // return if know which shape is next
    if (game_data->curr_idx < (TETROMINO_COUNT - 1))
        return;

    game_data->curr_idx = 0;

    shuffle(TETROMINO_COUNT - 1, game_data->nxt);
    ShapeId cache = game_data->nxt[0];
    game_data->nxt[0] = game_data->nxt[TETROMINO_COUNT - 1];
    game_data->nxt[TETROMINO_COUNT - 1] = cache;
}

void game_init(GameData* const game_data) {
    // zero-initialize the game data
    *game_data = (GameData){0};

    // allocate size for each row
    for (int8_t i = 0; i < ROWS; i++) {
        game_data->rows[i] = calloc(COLUMNS, sizeof(Colour8));
        // game_data->rows[i][0] = (colour8){(uint8_t)((((i + 1) ^ ((i + 1) >> 3)) * 0x27) & 0xFF)}; // for debugging rows
    }

    // set a random seed using the system clock
    srand(time(NULL));

    // set the shape data in each slot to it's corrsponding ID
    for (ShapeId i = 0; i < TETROMINO_COUNT; i++)
        game_data->nxt[i] = i;

    game_data->curr_idx = -1;                 // set the current index to the max so it becomes zero after increasement
    next_shape(game_data);                    // select the next shape (shuffle should not be triggered)
    shuffle(TETROMINO_COUNT, game_data->nxt); // manually trigger a shuffle
}

// called every time the game's state is updated
void game_update(GameData* const game_data, uint8_t const* const keys) {
    if (keys[SDL_SCANCODE_ESCAPE])
        stop();

    InputData move = MOVE_NONE;
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) move |= MOVE_LEFT;
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) move |= MOVE_RIGHT;
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_SPACE]) move |= MOVE_DOWN;
    if (keys[SDL_SCANCODE_Q]) move |= MOVE_ROTLEFT;
    if (keys[SDL_SCANCODE_E]) move |= MOVE_ROTRIGHT;
    place_update(game_data, move);
}

void game_free(GameData* const game_data) {
    // clear each row
    for (int8_t i = 0; i < ROWS; i++) {
        free(game_data->rows[i]);
        game_data->rows[i] = NULL;
    }
}
