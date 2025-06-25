#pragma once

#include <SDL_video.h>
#include <stdint.h>
#include <sys/cdefs.h>

#include "../game/game.h"

#define PX_DENS       25                                     // pixel density; pixels per block
#define TET_PADDING   10                                     // padding around the tetris playing field
#define TET_WIDTH     (COLUMNS * PX_DENS - TET_PADDING)      // tetris playing field width
#define TET_HEIGHT    (TET_WIDTH / COLUMNS * ROWS)           // tetris playing field height
#define SCREEN_WIDTH  ((COLUMNS + 6) * PX_DENS)              // window width
#define SCREEN_HEIGHT ((COLUMNS) * PX_DENS / COLUMNS * ROWS) // window height
#define BLOCK_WIDTH   (TET_WIDTH / COLUMNS)                  // width of a block
#define BLOCK_HEIGHT  (TET_HEIGHT / ROWS)                    // height of a block

__nonnull((1, 2)) void render_init(SDL_Window*, struct gamedata const*);
void render_update(void); // causes a draw to occur, will also determine update rate
void render_free(void);   // frees the memory allocated to the renderer in render_data
