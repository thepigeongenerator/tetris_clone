#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../game.h"

/* updates the movement of the pdat structure, updating the rows when colliding downwards.
 * closes window when the next shape intersects with the current one */
void place_update(struct gamedata* gdat, int movdat);
