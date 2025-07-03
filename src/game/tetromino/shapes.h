#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "../../io/colour/colour8.h"
#include "../../util/types.h"
#include "../../util/vec.h"

enum tetromino {
	TET_O = 0,
	TET_I = 1,
	TET_S = 2,
	TET_Z = 3,
	TET_T = 4,
	TET_L = 5,
	TET_J = 6,
	TET_R0 = 0,
	TET_R90 = 8,
	TET_R180 = 16,
	TET_R270 = 24,
};

#define SHAPE_WIDTH  4
#define SHAPE_HEIGHT 4
#define TETC         7

void shape_getblocks(u8 id, i8vec2* out);
colour8 colour_from_id(u8 id);
