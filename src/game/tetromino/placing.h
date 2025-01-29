#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../game.h"
#include "shapes.h"

void place_update(GameData* game_data, const uint8_t* keys);

#ifdef DEBUG
void dbg_set_all(GameData* game_data);
#endif
