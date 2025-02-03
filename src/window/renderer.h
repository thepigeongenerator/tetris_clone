#pragma once
#include <SDL_render.h>
#include <SDL_video.h>
#include <stdint.h>

#include "../game/game.h"

#define SCREEN_RESOLUTION 25                                // pixels per block
#define SCREEN_WIDTH      ((COLUMNS) * 25)                  // window width
#define SCREEN_HEIGHT     (SCREEN_WIDTH / COLUMNS * ROWS)   // window height
#define TET_PADDING       10                                // padding around the tetris playing field
#define TET_WIDTH         (SCREEN_WIDTH - TET_PADDING * 2)  // tetris playing field width
#define TET_HEIGHT        (SCREEN_HEIGHT - TET_PADDING * 2) // tetris playing field height
#define BLOCK_WIDTH       (TET_WIDTH / COLUMNS)             // width of a block
#define BLOCK_HEIGHT      (TET_HEIGHT / ROWS)               // height of a block

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    GameData* game_data;
} RenderData;

int renderer_init(SDL_Window** window, SDL_Renderer** renderer);
void renderer_update(const RenderData* render_data);
