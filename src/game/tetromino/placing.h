#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../game.h"

/* updates the movement of the `pdat` structure, updating the rows when colliding downwards.
 * returns `0` if we successfully updated. Returns 1 if we couldn't update. (e.g. when a next block immediately collides) */
int place_update(struct gamedata* gdat, int movdat);
