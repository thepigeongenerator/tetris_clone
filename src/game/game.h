#pragma once
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../io/colour/colour8.h"
#include "../util/types.h"
#include "../util/vec.h"
#include "tetromino/shapes.h"

// constants for pi(π) and tau(τ)
#define PI   (M_PI)         // π constant
#define TAU  (M_PI * 2.0)   // τ constant
#define PIf  (M_PIf)        // π constant as a 32-bit floating point
#define TAUf (M_PIf * 2.0F) // τ constant as a 32-bit floating point

// stores the data used in the game
#define COLUMNS 10
#define ROWS    24

/* contains the placement data */
struct pdat {
	u8 nxt[TETROMINO_COUNT]; // shuffled data representing the next shapes
	i8vec2 sel;              // position of the current shape
	u8 idx;                  // the index of the current shape
	u8 cur;                  // the current id of the shape
};

/* contains game data that's commonly shared */
struct gamedata {
	struct pdat pdat;
	colour8* rows[ROWS];
	u16 pnts;
};


/* increments to the next shape, shuffling the next shapes, if there isn't a next shape immediately after the current one. */
void next_shape(void);

struct gamedata* game_init(void);
void game_update(int movdat, size_t time);
void game_free(void);
