#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "../../io/colour/colour8.h"
#include "../../util/types.h"
#include "../../util/vec.h"

enum {
	TETROMINO_O = 0,
	TETROMINO_I = 1,
	TETROMINO_S = 2,
	TETROMINO_Z = 3,
	TETROMINO_T = 4,
	TETROMINO_L = 5,
	TETROMINO_J = 6,
	TETROMINO_ROTATED_0 = 0,
	TETROMINO_ROTATED_90 = 8,
	TETROMINO_ROTATED_180 = 16,
	TETROMINO_ROTATED_270 = 24,
};

#define SHAPE_WIDTH     4
#define SHAPE_HEIGHT    4
#define TETROMINO_COUNT 7

void shape_getblocks(u8 id, i8vec2* out);
colour8 colour_from_id(u8 id);
