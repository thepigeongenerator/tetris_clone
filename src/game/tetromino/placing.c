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


/* count the empty columns in the row */
__attribute_const__ static int count_empty(u8 const* restrict row) {
	int cnt = 0;
#if defined(__clang__)
#pragma unroll COLUMNS
#elif defined(__GNUC__)
#pragma GCC unroll COLUMNS
#endif
	for (int i = 0; i < COLUMNS; i++)
		cnt += !row[i];
	return cnt;
}

/* checks for filled rows, clearing a max of 4 consecutive rows, if present.
 * all row pointers will be moved down to fill the gap, the filled rows will be inserted at the top, and zeroed-out */
static void clear_rows(u8* restrict* restrict rows, u16* const score) {
	u8* cache[4] = {0}; // the I piece is 4 long, meaning only 4 rows can be cleared at a time
	uint fillc = 0;
	uint checkc = 0;

	for (uint y = 0; y < (ROWS - fillc) && checkc <= 4; y++) {
		int i = (ROWS - 1) - y;    // invert the index direction, so we start at the highest index
		rows[i] = rows[i - fillc]; // either assigns the current row to itself, or to the "next" row

		if (count_empty(rows[i])) {
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
static void plcmnt_place(u8* restrict const* restrict row, u8 id, i8vec2 pos) {
	u8 colour = colour_from_id(id);

	i8vec2 bpos[4];
	shape_getblocks(id, bpos);
	bpos[0] += pos;
	bpos[1] += pos;
	bpos[2] += pos;
	bpos[3] += pos;

	row[bpos[0][VY]][bpos[0][VX]] = colour;
	row[bpos[1][VY]][bpos[1][VX]] = colour;
	row[bpos[2][VY]][bpos[2][VX]] = colour;
	row[bpos[3][VY]][bpos[3][VX]] = colour;
}

static int plcmnt_valid(u8* restrict const* restrict const rows, i8vec2 pos) {
	return pos[VX] >= 0 && pos[VX] < COLUMNS &&
		pos[VY] >= 0 && pos[VY] < ROWS &&
		!rows[pos[VY]][pos[VX]];
}

static int plcmnt_intersect(u8* restrict const* restrict const rows, u8 const id, i8vec2 pos) {
	i8vec2 bpos[4];
	shape_getblocks(id, bpos);
	return !(plcmnt_valid(rows, pos + bpos[0]) &&
		plcmnt_valid(rows, pos + bpos[1]) &&
		plcmnt_valid(rows, pos + bpos[2]) &&
		plcmnt_valid(rows, pos + bpos[3]));
}

int place_update(struct gamedata* gdat, int movdat) {
	// store the current index and ID, only changes when placed (which yields no movement) and rotation (which occurs last)
	int tmp;
	u8 id = gdat->pdat.cur;

	// update Y axis
	tmp = !!(movdat & MOVD);
	gdat->pdat.sel[VY] += tmp;
	tmp = tmp && plcmnt_intersect(gdat->rows, id, gdat->pdat.sel);
	if (tmp) {
		gdat->pdat.sel[VY]--;
		plcmnt_place(gdat->rows, id, gdat->pdat.sel);
		clear_rows(gdat->rows, &gdat->pnts); // clear the rows that have been completed
		next_shape();
		audio_play(AUDIO_ID_PLACE);

		if (plcmnt_intersect(gdat->rows, gdat->pdat.cur, gdat->pdat.sel))
			return 1;
	}

	// update X axis
	tmp = !!(movdat & MOVR) - !!(movdat & MOVL);
	gdat->pdat.sel[VX] += (tmp && !plcmnt_intersect(gdat->rows, id, (i8vec2){gdat->pdat.sel[VX] + tmp, gdat->pdat.sel[VY]})) * tmp;

	// update roll
	tmp = id ^ (((!!(movdat & MOVRR) - !!(movdat & MOVRL)) * 8 + id) & 31);
	gdat->pdat.cur ^= (tmp && !plcmnt_intersect(gdat->rows, id ^ tmp, gdat->pdat.sel)) * tmp;
	return 0;
}
