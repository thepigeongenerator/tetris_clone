#include "placing.h"

#include <stdint.h>

#include "../../main.h"
#include "../../window/colour8.h"
#include "../game.h"
#include "shapes.h"


static bool is_filled(row_const const row) {
    for (int8_t x = 0; x < COLUMNS; x++) {
        if (row[x].packed == 0) {
            return false;
        }
    }

    return true;
}

static void clear_rows(row* const rows, uint16_t* const score) {
    row cache[4] = {0}; // you can only clear four rows at a time
    struct {
        uint8_t filled  : 3; // values will only ever be 0..4
        uint8_t checked : 3; // values will only ever be 0..4
    } dat = {0};

    // loop through each row (excluding the empty rows at the top when clearing a line)
    for (int8_t y = 0; y < (ROWS - dat.filled); y++) {
        int8_t const i = (ROWS - 1) - y; // get the index starting from the bottom

        rows[i] = rows[i - dat.filled]; // set the row to the new or same address

        // continue if the line isn't filled or the max amount has been reached
        if (dat.checked >= 4 || !is_filled(rows[i])) {
            if (dat.filled > 0 && dat.checked < 4) dat.checked++;
            continue; // continue to the next line
        }

        cache[dat.filled] = rows[i]; // cache the current row address
        dat.filled++;                // increase filled, and keep the row in the cache
        dat.checked++;               // increase the checked count
        y--;                         // decrease y to check this line again
    }

    if (dat.filled == 0) return;
    *score += 8 << dat.filled;

    // do while, as we already know that entering the loop that filled is non-zero
    do {
        dat.filled--;
        rows[dat.filled] = cache[dat.filled];

        // zero out the filled row
        for (int8_t x = 0; x < COLUMNS; x++)
            cache[dat.filled][x].packed = 0;
    } while (dat.filled > 0);
}

// sets a shape to the screen
static void set_shape_i(row const* const row, shape_id const id, int8_t const pos_x) {
    shape const shape = shape_from_id(id);
    colour8 const colour = colour_from_id(id);
    for (int8_t y = 0; y < SHAPE_HEIGHT; y++) {
        shape_row const shape_row = shape_get_row(shape, y);

        if (shape_row == 0)
            continue;

        for (int8_t x = 0; x < SHAPE_WIDTH; x++)
            if (shape_is_set(shape_row, x))
                row[y][x + pos_x] = colour;
    }
}

static inline void set_shape(row const* const row, shape_id const id, int8_t const pos_x, int8_t const pos_y) {
    set_shape_i(&row[pos_y], id, pos_x); // calls itself, but omitting the pos_y argument, instead opting for specifying the row
}

static bool shape_intersects(row const* const rows, shape_id const id, int8_t const x, int8_t const y) {
    shape const shape = shape_from_id(id);

    for (int8_t y0 = 0; y0 < SHAPE_HEIGHT; y0++) {
        shape_row const shape_row = shape_get_row(shape, y0); // get the shape row
        if (shape_row == 0) continue;                        // if the row doesn't contain data; continue

        for (int8_t x0 = 0; x0 < SHAPE_WIDTH; x0++) {
            if (shape_is_set(shape_row, x0) == false) continue; // if the bit isn't set at this index; continue
            int8_t const x1 = x + x0;
            int8_t const y1 = y + y0;

            if (x1 < 0 || x1 >= COLUMNS) return true;  // if X is out of bounds
            if (y1 < 0 || y1 >= ROWS) return true;     // if Y is out of bounds
            if (rows[y1][x1].packed != 0) return true; // if there is a block here
        }
    }
    return false;
}

static inline shape_id rotate_id(shape_id const id, int const dir) {
    return (id + dir) & 31;
}

void place_update(game_data* const game_data, input_data const move) {
    // store the current index and ID, only changes when placed (which yields no movement) and rotation (which occurs last)
    uint8_t const curr_idx = game_data->curr_idx;
    shape_id const curr_id = game_data->nxt[curr_idx];


    // set the shape if we moved vertically and intersected
    if (move & 4) {
        int8_t const y = game_data->sel_y + 1;
        if (shape_intersects(game_data->rows, curr_id, game_data->sel_x, y)) {
            set_shape(game_data->rows, curr_id, game_data->sel_x, game_data->sel_y); // if the shape intersects vertically, write the shape at the current position and return
            clear_rows(game_data->rows, &game_data->score);                          // clear the rows that have been completed

            next_shape(game_data);
            if (shape_intersects(game_data->rows, game_data->curr_idx, game_data->sel_x, game_data->sel_y))
                stop();
            return;
        }

        // otherwise, just set Y
        game_data->sel_y = y;
    }

    // update shape's X coordinate movement
    if ((move & 3) != 3 && (move & 3)) {
        int8_t const x = game_data->sel_x + ((move & 3) == 1 ? -1 : 1); // either move along -x or +x
        if (shape_intersects(game_data->rows, curr_id, x, game_data->sel_y) == false) {
            game_data->sel_x = x; // set X if the shape does not intersect
        }
    }

    // update the shape's rotation
    if (move & 8 || move & 16) {
        shape_id const id = move & 8 // check which direction we should move
                                ? rotate_id(curr_id, -8)
                                : rotate_id(curr_id, 8);
        if (shape_intersects(game_data->rows, id, game_data->sel_x, game_data->sel_y) == false) {
            game_data->nxt[curr_idx] = id;
        }
    }
}
