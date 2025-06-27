#include "game.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../io/colour/colour8.h"
#include "../io/input.h"
#include "../util/types.h"
#include "../util/vec.h"
#include "./tetromino/shapes.h"
#include "tetromino/placing.h"
#include "time.h"

static colour8 rowdat[COLUMNS * ROWS] = {0}; // contains the raw data of the rows, in no particular order
static struct gamedata dat = {0};

/* shuffle an array using the Fisher–Yates shuffle algorithm.
 * `nmemb` is the number of members.
 * `membs` is the byte size of each member */
static void shuffle(void* restrict ptr, size_t nmemb, size_t membs) {
	u8 dat[membs];

	for (size_t i = 0; i < nmemb; i++) {
		size_t j = i + rand() % (nmemb - i);
		void* ptri = (u8*)ptr + i * membs;
		void* ptrj = (u8*)ptr + j * membs;
		memcpy(dat, ptri, membs);
		memcpy(ptri, ptrj, membs);
		memcpy(ptrj, dat, membs);
	}
}

void next_shape(void) {
	// as long as we're not at the last shape, we can just increment
	dat.pdat.sel = (i8vec2){COLUMNS / 2 - SHAPE_WIDTH / 2, 0};
	dat.pdat.cur = dat.pdat.nxt[dat.pdat.idx];
	dat.pdat.idx++;
	if (dat.pdat.idx < TETROMINO_COUNT) return;

	// shuffle all next shapes, preserving the last
	dat.pdat.idx = 0;
	shuffle(dat.pdat.nxt, TETROMINO_COUNT, sizeof(u8));
}

struct gamedata* game_init(void) {
	srand(time(NULL));

	// populate the data arrays
	for (int i = 0; i < ROWS; i++)
		dat.rows[i] = rowdat + i * COLUMNS;
	for (int i = 0; i < TETROMINO_COUNT; i++)
		dat.pdat.nxt[i] = i;

	// initialise the placing data correctly
	dat.pdat.sel = (i8vec2){COLUMNS / 2 - SHAPE_WIDTH / 2, 0};
	shuffle(dat.pdat.nxt, TETROMINO_COUNT, sizeof(u8));
	dat.pdat.cur = dat.pdat.nxt[dat.pdat.idx];
	dat.pdat.idx++;
	return &dat;
}

// called every time the game's state is updated
void game_update(int movdat, size_t time) {
	static time_t drop_timeout = 0;
	movdat |= !!time_poll(time, 200, &drop_timeout) * MOVD;
	place_update(&dat, movdat);
}

void game_free(void) {
}
