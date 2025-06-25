#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../game.h"

// TODO: this singular function does too much, break it up via different returns and such.
/* updates the movement of the pdat structure, updating the rows when colliding downwards.
 * closes window when the next shape intersects with the current one */
void place_update(struct gamedata* gdat, int movdat);
