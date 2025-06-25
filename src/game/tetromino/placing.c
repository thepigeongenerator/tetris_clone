#include "placing.h"

#include <stdint.h>

#include "../../io/audio.h"
#include "../../io/colour/colour8.h"
#include "../../util/types.h"
#include "../game.h"
#include "shapes.h"


static int is_filled(row_const const row) {
	for (int8_t x = 0; x < COLUMNS; x++) {
		if (row[x] == 0) {
			return 0;
		}
	}

	return 1;
}

static void clear_rows(row* const rows, uint16_t* const score) {
	row cache[4] = {0}; // you can only clear four rows at a time
	unsigned filled = 0;
	unsigned checked = 0;

	// loop through each row (excluding the empty rows at the top when clearing a line)
	for (unsigned y = 0; y < (ROWS - filled); y++) {
		int const i = (ROWS - 1) - y; // get the index starting from the bottom

		rows[i] = rows[i - filled]; // set the row to the new or same address

		// continue if the line isn't filled or the max amount has been reached
		if (checked >= 4 || !is_filled(rows[i])) {
			if (filled > 0 && checked < 4) checked++;
			continue; // continue to the next line
		}

		cache[filled] = rows[i]; // cache the current row address
		filled++;                // increase filled, and keep the row in the cache
		checked++;               // increase the checked count
		y--;                     // decrease y to check this line again
	}

	if (filled == 0) return;
	*score += 8 << filled;

	// do while, as we already know that entering the loop that filled is non-zero
	do {
		filled--;
		rows[filled] = cache[filled];

		// zero out the filled row
		for (unsigned x = 0; x < COLUMNS; x++)
			cache[filled][x] = 0;
	} while (filled > 0);
}

// sets a shape to the screen
static void set_shape_i(row const* const row, u8 const id, int8_t const pos_x) {
	u16 const shape = shape_from_id(id);
	colour8 const colour = colour_from_id(id);
	for (int8_t y = 0; y < SHAPE_HEIGHT; y++) {
		u8 const shape_row = shape_get_row(shape, y);
// TODO: this is suboptimal, ditch the entire "representing shapes as binary-formatted data" and instead use a switch...case.

		if (shape_row == 0)
			continue;

		for (int8_t x = 0; x < SHAPE_WIDTH; x++)
			if (shape_is_set(shape_row, x))
				row[y][x + pos_x] = colour;
	}
}

static inline void set_shape(row const* const row, u8 const id, int8_t const pos_x, int8_t const pos_y) {
	set_shape_i(&row[pos_y], id, pos_x); // calls itself, but omitting the pos_y argument, instead opting for specifying the row
}

static int shape_intersects(row const* const rows, u8 const id, int8_t const x, int8_t const y) {
	u16 const shape = shape_from_id(id);

	for (int y0 = 0; y0 < SHAPE_HEIGHT; y0++) {
		u8 const shape_row = shape_get_row(shape, y0); // get the shape row
		if (shape_row == 0) continue;                  // if the row doesn't contain data; continue

		for (int x0 = 0; x0 < SHAPE_WIDTH; x0++) {
			if (shape_is_set(shape_row, x0) == false) continue; // if the bit isn't set at this index; continue
			int const x1 = x + x0;
			int const y1 = y + y0;

			if (x1 < 0 || x1 >= COLUMNS) return 1; // if X is out of bounds
			if (y1 < 0 || y1 >= ROWS) return 1;    // if Y is out of bounds
			if (rows[y1][x1] != 0) return 1;       // if there is a block here
		}
	}
	return 0;
}

static inline u8 rotate_id(u8 const id, int const dir) {
	return (id + dir) & 31;
}

void place_update(gamedata* const game_data, input_data const move) {
	// store the current index and ID, only changes when placed (which yields no movement) and rotation (which occurs last)
	uint8_t const curr_idx = game_data->curr_idx;
	u8 curr_id = game_data->nxt[curr_idx];


	// set the shape if we moved vertically and intersected
	if (move & 4) {
		int8_t const y = game_data->sel_y + 1;
		if (shape_intersects(game_data->rows, curr_id, game_data->sel_x, y)) {
			set_shape(game_data->rows, curr_id, game_data->sel_x, game_data->sel_y); // if the shape intersects vertically, write the shape at the current position and return
			clear_rows(game_data->rows, &game_data->score);                          // clear the rows that have been completed

			audio_play(&game_data->place_sfx);

			next_shape(game_data);
			if (shape_intersects(game_data->rows, game_data->curr_idx, game_data->sel_x, game_data->sel_y))
				game_data->run = false;
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
		u8 const id = move & 8 // check which direction we should move
			? rotate_id(curr_id, -8)
			: rotate_id(curr_id, 8);
		if (shape_intersects(game_data->rows, id, game_data->sel_x, game_data->sel_y) == false) {
			game_data->nxt[curr_idx] = id;
		}
	}
}
