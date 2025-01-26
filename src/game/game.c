#include "game.h"

#include <SDL_scancode.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../main.h"
#include "../window/colour.h"
#include "./tetromino/shapes.h"
#include "tetromino/placing.h"


static void clear_row(Row* row, uint8_t y) {
    // move the rows down from the specified height
    for (; y > 0; y--)
        memcpy(row[y].columns, row[y - 1].columns, COLUMNS * sizeof(Colour));

    memset(row[0].columns, NONE, sizeof(Colour));
}

static void clear_rows(Row* row) {
    // start at the lowest row
    // integer will wrap
    for (uint8_t y = ROWS - 1; y < ROWS; y--) {
        Row* crow = &row[y];  // the current row
        bool nfilled = false; // whether this row is filled

        // loop through all the columns or till nfilled is true
        for (uint8_t x = 0; x < COLUMNS; x++) {
            if (crow->columns[x].packed == NONE) {
                nfilled = true;
                break;
            }
        }

        // continue if the row isn;t filled
        if (!nfilled)
            clear_row(crow, y);
    }
}

// called every time the game's state is updated
void game_update(GameData* game_data, const uint8_t* keys) {
    if (keys[SDL_SCANCODE_ESCAPE]) {
        stop();
    }

    //tmp_set_random(game_data);
    tmp_set_all(game_data);
}
