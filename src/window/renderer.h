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

struct render_cache {
    SDL_Texture* score_texture;
    SDL_Surface* score_surface;
    uint16_t prevscore;
};

typedef struct {
    game_data const* game_dat;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    struct render_cache* cache;
} render_data;

void renderer_init(render_data* render_dat, game_data const* game_dat);
void renderer_update(render_data const* dat);
void renderer_free(render_data* dat);
