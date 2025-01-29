#include "placing.h"

#include <stdint.h>
#include <stdlib.h>

#include "../../window/colour.h"
#include "../game.h"
#include "SDL_scancode.h"
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

static bool shape_intersects(const Row* rows, const ShapeId id, const uint8_t x, const uint8_t y) {
    const Shape shape = shape_from_id(id);

    for (uint8_t y0 = 0; y0 < SHAPE_HEIGHT; y0++) {
        const ShapeRow shape_row = shape_get_row(shape, y0); // get the shape row
        if (shape_row == 0) continue;                        // if the row doesn't contain data; continue

        for (uint8_t x0 = 0; x0 < SHAPE_WIDTH; x0++) {
            if (is_set(shape_row, x0) == false) continue; // if the bit isn't set at this index; continue
            const uint8_t x1 = x + x0;
            const uint8_t y1 = y + y0;

            if (x1 < 0 || x1 >= COLUMNS) return true;  // if X is out of bounds
            if (y1 < 0 || y1 >= ROWS) return true;     // if Y is out of bounds
            if (rows[y1][x1].packed != 0) return true; // if there is a block here
        }
    }
    return false;
}

void place_update(GameData* const game_data, const uint8_t* const keys) {
    int8_t move = 0;

    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A])
        move |= 1;

    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D])
        move |= 2;

    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_SPACE])
        move |= 4;

    // set the shape if we moved vertically and collided
    if (!!(move & 4)) {
        const uint8_t y = game_data->selected.y + 1;
        if (shape_intersects(game_data->row, game_data->selected.id, game_data->selected.x, y)) {
            set_shape(game_data->row, game_data->selected.id, game_data->selected.x, y);
            return;
        }

        game_data->selected.y = y;
    }

    // ignore if the movement cancles itself out
    if ((move & 3) == 0 || (move & 3) == 3)
        return;

    const uint8_t x = game_data->selected.x + ((move & 3) == 1 ? -1 : 1);
    if (shape_intersects(game_data->row, game_data->selected.id, x, game_data->selected.y))
        return;

    game_data->selected.x = x;
    printf("%i\n", x);
}

#ifdef DEBUG
void dbg_set_all(GameData* game_data) {
    for (uint8_t i = 0; i < TETROMINO_COUNT; i++)
        for (uint8_t r = 0; r < 4; r++)
            set_shape(game_data->row, i | (r << 3), r * 4, i * 4);
}
#endif
