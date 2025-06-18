#pragma once

#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <SDL_video.h>
#include <stdint.h>

#include "../game/game.h"

#define PX_DENS       25                                     // pixel density; pixels per block
#define TET_PADDING   10                                     // padding around the tetris playing field
#define TET_WIDTH     (COLUMNS * PX_DENS - TET_PADDING)      // tetris playing field width
#define TET_HEIGHT    (TET_WIDTH / COLUMNS * ROWS)           // tetris playing field height
#define SCREEN_WIDTH  ((COLUMNS + 6) * PX_DENS)              // window width
#define SCREEN_HEIGHT ((COLUMNS) * PX_DENS / COLUMNS * ROWS) // window height
#define BLOCK_WIDTH   (TET_WIDTH / COLUMNS)                  // width of a block
#define BLOCK_HEIGHT  (TET_HEIGHT / ROWS)                    // height of a block

// contains the data that's cached between renders
struct render_cache {
	SDL_Texture* score_texture;
	SDL_Surface* score_surface;
	uint16_t prevscore;
};

// contains the data necessary for rendering
typedef struct {
	gamedata const* game_dat;
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	struct render_cache* cache;
} renderdata;

void render_init(renderdata*, gamedata const*); // initializes the renderer, outputs to render_data
void render_update(renderdata const*);          // causes a draw to occur, will also determine update rate
void render_free(renderdata*);                  // frees the memory allocated to the renderer in render_data
