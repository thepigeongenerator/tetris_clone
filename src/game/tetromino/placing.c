#include "placing.h"

#include <stdint.h>
#include <stdio.h>

#include "../../window/colour.h"
#include "../game.h"
#include "shapes.h"


static bool is_filled(CRow row) {
    for (int8_t x = 0; x < COLUMNS; x++) {
        if (row[x].packed == NONE) {
            return false;
        }
    }

    return true;
}

static void clear_rows(Row* row) {
    int8_t filled = 0;

    // loop through each row (excluding the empty rows at the top when clearing a line)
    for (int8_t y = 0; y < (ROWS - filled); y++) {
        const int8_t i = (ROWS - 1) - y; // get the index starting from the bottom

        Row row_cache = row[i];   // cache this row address in case the row is filled
        row[i] = row[i - filled]; // set the row to the new or same address

        if (!is_filled(row[i])) continue;

        // zero out the cache
        for (int8_t x = 0; x < COLUMNS; x++)
            row[i][x].packed = 0;

        // write the cached address to a row starting from the top
        row[filled++] = row_cache;
        row[i] = row[i - filled];
        y--;
    }

    if (filled > 0)
        printf("filled: %i\n", filled);
}

// sets a shape to the screen
static void set_shape_i(Row* row, const ShapeId id, const int8_t pos_x) {
    const Shape shape = shape_from_id(id);
    const Colour colour = colour_from_id(id);
    for (int8_t y = 0; y < SHAPE_HEIGHT; y++) {
        ShapeRow shape_row = shape_get_row(shape, y);

        if (shape_row == 0)
            continue;

        for (int8_t x = 0; x < SHAPE_WIDTH; x++)
            if (is_set(shape_row, x))
                row[y][x + pos_x] = colour;
    }
}

static inline void set_shape(Row* row, const ShapeId id, const int8_t pos_x, const int8_t pos_y) {
    set_shape_i(&row[pos_y], id, pos_x); // calls itself, but omitting the pos_y argument, instead opting for specifying the row
}

static bool shape_intersects(const Row* rows, const ShapeId id, const int8_t x, const int8_t y) {
    const Shape shape = shape_from_id(id);

    for (int8_t y0 = 0; y0 < SHAPE_HEIGHT; y0++) {
        const ShapeRow shape_row = shape_get_row(shape, y0); // get the shape row
        if (shape_row == 0) continue;                        // if the row doesn't contain data; continue

        for (int8_t x0 = 0; x0 < SHAPE_WIDTH; x0++) {
            if (is_set(shape_row, x0) == false) continue; // if the bit isn't set at this index; continue
            const int8_t x1 = x + x0;
            const int8_t y1 = y + y0;

            if (x1 < 0 || x1 >= COLUMNS) return true;  // if X is out of bounds
            if (y1 < 0 || y1 >= ROWS) return true;     // if Y is out of bounds
            if (rows[y1][x1].packed != 0) return true; // if there is a block here
        }
    }
    return false;
}

static inline ShapeId rotate_id(const ShapeId id, const int dir) {
    return (id + dir) & 31;
}

void place_update(GameData* const game_data, const InputData move) {
    SelectedShape* const selected = &game_data->selected;
    // set the shape if we moved vertically and intersected
    if (move & 4) {
        const int8_t y = selected->y + 1;
        if (shape_intersects(game_data->row, selected->id, selected->x, y)) {
            set_shape(game_data->row, selected->id, selected->x, selected->y); // if the shape intersects vertically, write the shape at the current position and return
            clear_rows(game_data->row);                                        // clear the rows that have been completed

            game_data->selected = (SelectedShape){game_data->next_shape, COLUMNS / 2 - SHAPE_WIDTH / 2, 0};
            set_next_shape(game_data);
            return;
        }

        // otherwise, just set Y
        selected->y = y;
    }

    // update shape's X coordinate movement
    if ((move & 3) != 3 && (move & 3)) {
        const int8_t x = selected->x + ((move & 3) == 1 ? -1 : 1); // either move along -x or +x
        if (shape_intersects(game_data->row, selected->id, x, selected->y) == false) {
            selected->x = x; // set X if the shape does not intersect
        }
    }

    // update the shape's rotation
    if (move & 8 || move & 16) {
        const ShapeId id = move & 8 ? rotate_id(selected->id, -8) : rotate_id(selected->id, 8);
        if (shape_intersects(game_data->row, id, selected->x, selected->y) == false) {
            selected->id = id;
        }
    }
}

#ifdef DEBUG
void dbg_set_all(GameData* game_data) {
    for (uint8_t i = 0; i < TETROMINO_COUNT; i++)
        for (uint8_t r = 0; r < 4; r++)
            set_shape(game_data->row, i | (r << 3), r * 4, i * 4);
}
#endif
