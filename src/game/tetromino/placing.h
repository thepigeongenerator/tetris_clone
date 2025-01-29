#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../game.h"
#include "shapes.h"

typedef uint8_t InputData;
enum {
    MOVE_NONE = 0,
    MOVE_LEFT = 1,
    MOVE_RIGHT = 2,
    MOVE_DOWN = 4,
    MOVE_ROTLEFT = 8,
    MOVE_ROTRIGHT = 16,
};

void place_update(GameData* game_data, const InputData move);

#ifdef DEBUG
void dbg_set_all(GameData* game_data);
#endif
