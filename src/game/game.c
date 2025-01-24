#include "game.h"

#include <stdbool.h>

#include "../main.h"
#include "shapes.h"


// sets a shape to the screen
static void _set_shape(Row* row, const Shape shape, const Colour colour, const uint8_t pos_x) {
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
    _set_shape(&row[pos_y], shape, colour, pos_x); // calls itself, but omitting the pos_y argument, instead opting for specifying the row
}

// called every time the game's state is updated
void game_update(GameData* game_data, const Uint8* keys) {
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
