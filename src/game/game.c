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


void game_init(GameData* const game_data) {
    // zero-initialize the game data
    *game_data = (GameData){0};

    // write the pointer information for the rows
    for (uint8_t i = 0; i < ROWS; i++)
        game_data->row[i] = game_data->row_raw[i];

    game_data->selected = (SelectedShape){TETROMINO_L, 0, 0};

    // set a random seed using the system clock
    srand(time(NULL));
}

static bool is_filled(CRow row) {
    for (uint8_t x = 0; x < COLUMNS; x++) {
        if (row[x].packed == NONE) {
            return false;
        }
    }

    return true;
}

static inline void set_row(Row* rows, const uint8_t y, Row row) {
    rows[(ROWS - 1) - y] = row;
}

static void clear_rows(Row* row) {
    uint8_t filled_rows = 0;

    // loop through each row
    for (uint8_t y = 0; y < ROWS; y++) {
        // zero out the row instead if we've reached the top rows
        if ((ROWS - y) <= filled_rows) {
            for (uint8_t x = 0; x < COLUMNS; x++)
                row[filled_rows--][x].packed = 0; // use filled_rows as the y index and decrease them as we go.
            continue;
        }

        // get the row we currently need to check
        Row crow = filled_rows > 0
                       ? row[y + filled_rows]
                       : row[y];

        // set the current y to the current row
        Row row_cache = row[y];
        set_row(row, y, crow);

        if (!is_filled(crow)) continue;

        // write the cached row to the top if we know that the current row pointer will be lost
        row[filled_rows] = row_cache;
        filled_rows++;
    }
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
