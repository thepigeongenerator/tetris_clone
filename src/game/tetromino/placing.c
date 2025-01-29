#include "placing.h"

#include <stdint.h>
#include <stdlib.h>

#include "../../window/colour.h"
#include "../game.h"
#include "shapes.h"

// sets a shape to the screen
static void set_shape_i(Row* row, const ShapeId id, const uint8_t pos_x) {
    const Shape shape = shape_from_id(id);
    const Colour colour = colour_from_id(id);
    for (uint8_t y = 0; y < SHAPE_HEIGHT; y++) {
        ShapeRow shape_row = shape_get_row(shape, y);

        if (shape_row == 0)
            continue;

        for (uint8_t x = 0; x < SHAPE_WIDTH; x++)
            if (is_set(shape_row, x))
                row[y][x + pos_x] = colour;
    }
}

static inline void set_shape(Row* row, const ShapeId id, const uint8_t pos_x, const uint8_t pos_y) {
    set_shape_i(&row[pos_y], id, pos_x); // calls itself, but omitting the pos_y argument, instead opting for specifying the row
}

#ifdef DEBUG
void dbg_set_all(GameData* game_data) {
    for (uint8_t i = 0; i < TETROMINO_COUNT; i++)
        for (uint8_t r = 0; r < 4; r++)
            set_shape(game_data->row, i | (r << 3), r * 4, i * 4);
}
#endif

void tmp_set_random(GameData* game_data) {
    static int finished = 0;
    if (finished)
        return;

    finished = 1;
    for (uint8_t y = 0; y <= ROWS - SHAPE_HEIGHT; y += SHAPE_HEIGHT) {
        for (uint8_t x = 0; x <= COLUMNS - SHAPE_WIDTH; x += SHAPE_WIDTH) {
            const ShapeId id = (rand() % TETROMINO_COUNT) | ((rand() % 4) << 3);
            set_shape(game_data->row, id, x, y);
        }
    }
}
