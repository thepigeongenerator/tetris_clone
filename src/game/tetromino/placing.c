#include "placing.h"

#include <stdint.h>
#include <stdlib.h>

#include "../../window/colour.h"
#include "../game.h"
#include "shapes.h"

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

void tmp_set_all(GameData* game_data) {
    set_shape(game_data->row, TETROMINO_I, COLOUR_CYAN, 1, 0);
    set_shape(game_data->row, TETROMINO_O, COLOUR_YELLOW, 5, 4);

    set_shape(game_data->row, TETROMINO_Z, COLOUR_GREEN, 1, 8);
    set_shape(game_data->row, TETROMINO_S, COLOUR_RED, 5, 8);

    set_shape(game_data->row, TETROMINO_J, COLOUR_BLUE, 1, 12);
    set_shape(game_data->row, TETROMINO_L, COLOUR_ORANGE, 5, 12);

    set_shape(game_data->row, TETROMINO_T, COLOUR_MAGENTA, 5, 16);
}

void tmp_set_random(GameData* game_data) {
    uint32_t x = rand() % TETROMINO_COUNT;
    Shape shape = 0;
    Colour colour = {0};
    switch (x) {
    case 0:
        shape = TETROMINO_I;
        colour = COLOUR_CYAN;
        break;
    case 1:
        shape = TETROMINO_J;
        colour = COLOUR_BLUE;
        break;
    case 2:
        shape = TETROMINO_L;
        colour = COLOUR_ORANGE;
        break;
    case 3:
        shape = TETROMINO_O;
        colour = COLOUR_YELLOW;
        break;
    case 4:
        shape = TETROMINO_S;
        colour = COLOUR_RED;
        break;
    case 5:
        shape = TETROMINO_T;
        colour = COLOUR_MAGENTA;
        break;
    case 6:
        shape = TETROMINO_Z;
        colour = COLOUR_GREEN;
        break;
    }

    set_shape(game_data->row, shape, colour, 1, 1);
}
