#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "../../io/colour/colour8.h"
#include "../../util/types.h"

enum {
	TETROMINO_O = 0,
	TETROMINO_I = 1,
	TETROMINO_S = 2,
	TETROMINO_Z = 3,
	TETROMINO_T = 4,
	TETROMINO_L = 5,
	TETROMINO_J = 6,
	TETROMINO_ROTATED_90 = 8,
	TETROMINO_ROTATED_180 = 16,
	TETROMINO_ROTATED_270 = 24,
};

#define SHAPE_WIDTH     4
#define SHAPE_HEIGHT    4
#define TETROMINO_COUNT 7



static inline u8 shape_get_row(u16 shape, u8 index) {
	return shape >> (((SHAPE_HEIGHT - 1) - index) * SHAPE_WIDTH) & 0xF;
}

static inline bool shape_is_set(u8 row, u8 index) {
	return (row >> ((SHAPE_WIDTH - 1) - index) & 1) != 0;
}

u16 shape_from_id(u8 id);
colour8 colour_from_id(u8 id);
