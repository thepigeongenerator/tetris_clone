#include "game.h"

#include <SDL_scancode.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../main.h"
#include "../window/colour.h"
#include "shapes.h"


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

// sets a shape to the screen
static void set_shape_i(Row* row, const Shape shape, const Colour colour, const uint8_t pos_x) {
    for (uint8_t y = 0; y < SHAPE_HEIGHT; y++) {
        ShapeRow shape_row = shape_get_row(shape, y);

        if (shape_row == 0)
            continue;

        for (uint8_t x = 0; x < SHAPE_WIDTH; x++)
            if (is_set(shape_row, x))
                row[y].columns[x + pos_x] = colour;
    }
}

static inline void set_shape(Row* row, const Shape shape, const Colour colour, const uint8_t pos_x, const uint8_t pos_y) {
    set_shape_i(&row[pos_y], shape, colour, pos_x); // calls itself, but omitting the pos_y argument, instead opting for specifying the row
}

// called every time the game's state is updated
void game_update(GameData* game_data, const uint8_t* keys) {
    if (keys[SDL_SCANCODE_ESCAPE]) {
        stop();
    }

    set_shape(game_data->row, TETROMINO_I, COLOUR_CYAN, 1, 0);
    set_shape(game_data->row, TETROMINO_O, COLOUR_YELLOW, 5, 4);

    set_shape(game_data->row, TETROMINO_Z, COLOUR_GREEN, 1, 8);
    set_shape(game_data->row, TETROMINO_S, COLOUR_RED, 5, 8);

    set_shape(game_data->row, TETROMINO_J, COLOUR_BLUE, 1, 12);
    set_shape(game_data->row, TETROMINO_L, COLOUR_ORANGE, 5, 12);

    set_shape(game_data->row, TETROMINO_T, COLOUR_MAGENTA, 5, 16);
}
