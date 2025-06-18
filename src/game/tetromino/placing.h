#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../game.h"

typedef uint8_t input_data;
enum {
	MOVE_NONE = 0,
	MOVE_LEFT = 1,
	MOVE_RIGHT = 2,
	MOVE_DOWN = 4,
	MOVE_ROTLEFT = 8,
	MOVE_ROTRIGHT = 16,
};

void place_update(gamedata* game_data, input_data move);

#ifdef DEBUG
void dbg_set_all(gamedata* game_data);
#endif
