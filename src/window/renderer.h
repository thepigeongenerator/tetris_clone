#pragma once
#include <SDL2/SDL.h>
#include "../game/game.h"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT (SCREEN_WIDTH * 2)
#define BLOCK_WIDTH (SCREEN_WIDTH / 16)
#define BLOCK_HEIGHT (SCREEN_HEIGHT / (BLOCK_WIDTH * 2))

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    GameData* game_data;
} RenderData;

int renderer_init(SDL_Window** window, SDL_Renderer** renderer);
void renderer_update(const RenderData* render_data);
