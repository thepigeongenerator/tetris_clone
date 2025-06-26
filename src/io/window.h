#pragma once

#include "../game/game.h"

#define PX_DENS       25                                     // pixel density; pixels per block
#define SCREEN_WIDTH  ((COLUMNS + 6) * PX_DENS)              // window width
#define SCREEN_HEIGHT ((COLUMNS) * PX_DENS / COLUMNS * ROWS) // window height

void window_init(struct gamedata const*);
void window_open(void);
void window_close(void);
void window_free(void);
