#include "placing.h"

#include <stdint.h>
#include <string.h>
#include <sys/cdefs.h>

#include "../../io/audio.h"
#include "../../io/input.h"
#include "../../util/types.h"
#include "../../util/vec.h"
#include "../game.h"
#include "shapes.h"


/* checks if `row` of `COLUMNS` wide contains anything with `0`.
 * returns `1` if it doesn't, otherwise returns `0` */
__attribute_const__ static int is_filled(u8 const* restrict row) {
	int res = 0;
#if defined(__clang__)
#pragma unroll COLUMNS
#elif defined(__GNUC__)
#pragma GCC unroll COLUMNS
#endif
	for (int i = 0; i < COLUMNS; i++) res |= !row[i];
	return !res;
}

/* checks for filled rows, clearing a max of 4 consecutive rows, if present.
 * all row pointers will be moved down to fill the gap, the filled rows will be inserted at the top, and zeroed-out */
static void clear_rows(u8* restrict* restrict rows, u16* const score) {
	u8* cache[4] = {0}; // the I piece is 4 long, meaning only 4 rows can be cleared at a time
	uint fillc = 0;
	uint checkc = 0;

	for (uint y = 0; y < (ROWS - fillc); y++) {
		int i = (ROWS - 1) - y;    // invert the index direction, so we start at the highest index
		rows[i] = rows[i - fillc]; // either assigns the current row to itself, or to the "next" row

		if (checkc >= 4 || !is_filled(rows[i])) {
			checkc += (fillc > 0 && checkc < 4);
			continue;
		}

		cache[fillc] = rows[i]; // cache the current row address, since it's going to be overridden
		fillc++;
		checkc++;
		y--;
	}

	*score += (8 << fillc) * !!fillc;
	for (uint i = 0; i < fillc; i++) {
		memset(cache[i], 0, COLUMNS);
		rows[i] = cache[i];
	}
}

// TODO: this is suboptimal, ditch the entire "representing shapes as binary-formatted data" and instead use a switch...case.
/* writes a shape to the screen */
static void set_shape(u8* restrict const* restrict row, u8 id, int pos_x) {
	u16 shape = shape_from_id(id);
	u8 colour = colour_from_id(id);

	for (uint y = 0; y < SHAPE_HEIGHT; y++) {
		u8 const shape_row = shape_get_row(shape, y);

		for (int8_t x = 0; x < SHAPE_WIDTH; x++)
			if (shape_is_set(shape_row, x))
				row[y][x + pos_x] = colour;
	}
}

static int shape_intersects(u8* restrict const* restrict const rows, u8 const id, i8vec2 pos) {
	u16 const shape = shape_from_id(id);

	for (int y0 = 0; y0 < SHAPE_HEIGHT; y0++) {
		u8 const shape_row = shape_get_row(shape, y0); // get the shape row
		if (shape_row == 0) continue;                  // if the row doesn't contain data; continue

		for (int x0 = 0; x0 < SHAPE_WIDTH; x0++) {
			if (shape_is_set(shape_row, x0) == false) continue; // if the bit isn't set at this index; continue
			int x1 = pos[VX] + x0;
			int y1 = pos[VY] + y0;

			if (x1 < 0 || x1 >= COLUMNS) return 1; // if X is out of bounds
			if (y1 < 0 || y1 >= ROWS) return 1;    // if Y is out of bounds
			if (rows[y1][x1] != 0) return 1;       // if there is a block here
		}
	}
	return 0;
}

void place_update(struct gamedata* gdat, int movdat) {
	// store the current index and ID, only changes when placed (which yields no movement) and rotation (which occurs last)
	int tmp;
	u8 id = gdat->pdat.cur;

	// update Y axis
	tmp = !!(movdat & MOVD);
	gdat->pdat.sel[VY] += tmp;
	tmp = tmp && shape_intersects(gdat->rows, id, gdat->pdat.sel);
	if (tmp) {
		gdat->pdat.sel[VY]--;
		set_shape(gdat->rows + gdat->pdat.sel[VY], id, gdat->pdat.sel[VX]);
		clear_rows(gdat->rows, &gdat->pnts); // clear the rows that have been completed
		next_shape();
		audio_play(AUDIO_ID_PLACE);
	}

	// update X axis
	tmp = !!(movdat & MOVR) - !!(movdat & MOVL);
	gdat->pdat.sel[VX] += (tmp && !shape_intersects(gdat->rows, id, (i8vec2){gdat->pdat.sel[VX] + tmp, gdat->pdat.sel[VY]})) * tmp;

	// update roll
	tmp = id ^ (((!!(movdat & MOVRR) - !!(movdat & MOVRL)) * 8 + id) & 31);
	gdat->pdat.cur ^= (tmp && !shape_intersects(gdat->rows, id ^ tmp, gdat->pdat.sel)) * tmp;
}
