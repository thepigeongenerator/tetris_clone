#include "game.h"

#include <SDL_scancode.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../main.h"
#include "../window/colour.h"
#include "./tetromino/shapes.h"
#include "tetromino/placing.h"


void set_next_shape(GameData* const game_data) {
    ShapeId id = (rand() % TETROMINO_COUNT) | (rand() % 4 << 3);
    game_data->next_shape = id;
}

void game_init(GameData* const game_data) {
    // zero-initialize the game data
    *game_data = (GameData){0};

    // allocate size for each row
    for (int8_t i = 0; i < ROWS; i++) {
        game_data->rows[i] = calloc(COLUMNS, sizeof(Colour));
        //game_data->rows[i][0] = (Colour){(uint8_t)((((i + 1) ^ ((i + 1) >> 3)) * 0x27) & 0xFF)}; // for debugging
    }

    // set a random seed using the system clock
    srand(time(NULL));

    set_next_shape(game_data);
    game_data->selected = (SelectedShape){game_data->next_shape, COLUMNS / 2 - SHAPE_WIDTH / 2, 0};
    set_next_shape(game_data);
}

// called every time the game's state is updated
void game_update(GameData* game_data, const uint8_t* keys) {
    if (keys[SDL_SCANCODE_ESCAPE])
        stop();

    InputData move = MOVE_NONE;
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) move |= MOVE_LEFT;
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) move |= MOVE_RIGHT;
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_SPACE]) move |= MOVE_DOWN;
    if (keys[SDL_SCANCODE_Q]) move |= MOVE_ROTLEFT;
    if (keys[SDL_SCANCODE_E]) move |= MOVE_ROTRIGHT;
    place_update(game_data, move);

    // dbg_set_all(game_data);
}

void game_free(GameData* const game_data) {
    // clear each row
    for (int8_t i = 0; i < ROWS; i++) {
        free(game_data->rows[i]);
        game_data->rows[i] = NULL;
    }
}
