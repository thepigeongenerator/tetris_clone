#include "game.h"

#include <stdbool.h>

#include "../main.h"
#include "../window/renderer.h"
#include "shapes.h"


static void _add_shape(Row* row, const Shape shape, const uint8_t colour, const uint8_t pos_x) {
    for (uint8_t y = 0; y < SHAPE_HEIGHT; y++) {
        ShapeRow shape_row = shape_get_row(shape, y);

        if (shape_row == 0)
            continue;

        for (uint8_t x = 0; x < SHAPE_WIDTH; x++)
            if (is_set(shape_row, x))
                row[y].columns[x + pos_x].block = colour;
    }
}

static inline void add_shape(Row* row, const Shape shape, const uint8_t colour, const uint8_t pos_x, const uint8_t pos_y) {
    _add_shape(&row[pos_y], shape, colour, pos_x);
}

// called every time the game's state is updated
void game_update(GameData* game_data, const Uint8* keys) {
    if (keys[SDL_SCANCODE_ESCAPE]) {
        stop();
    }

    add_shape(game_data->row, TETROMINO_I, CYAN, 1, 0);
    add_shape(game_data->row, TETROMINO_O, YELLOW, 5, 4);

    add_shape(game_data->row, TETROMINO_Z, GREEN, 1, 8);
    add_shape(game_data->row, TETROMINO_S, RED, 5, 8);

    add_shape(game_data->row, TETROMINO_J, BLUE, 1, 12);
    add_shape(game_data->row, TETROMINO_L, YELLOW | RED, 5, 12);

    add_shape(game_data->row, TETROMINO_T, MAGENTA, 5, 16);
}
