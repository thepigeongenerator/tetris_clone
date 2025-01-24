// initializes the window and renderer
#include "renderer.h"

#include <SDL_error.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <stdint.h>
#include <stdio.h>

#include "../errors.h"
#include "../game/game.h"
#include "colour.h"
#include "renderer.h"


int renderer_init(SDL_Window** window, SDL_Renderer** renderer) {
    // create a new window
    *window = SDL_CreateWindow("tetris clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        error(ERROR_SDL_RENDERER_INIT, "Window failed to be created! SDL Error: %s", SDL_GetError());
        return -1;
    }

    // create a renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        error(ERROR_SDL_RENDERER_INIT, "Renderer failed to be created! SDL Error: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

void renderer_update(const RenderData* render_data) {
    SDL_Renderer* renderer = render_data->renderer;
    GameData* data = render_data->game_data;

    int success = 0; // if an error occurs, this value is <0

    // clear render
    success |= SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    success |= SDL_RenderClear(renderer);

    for (uint8_t y = 0; y < ROWS; y++) {
        Row row = data->row[y];

        for (uint8_t x = 0; x < COLUMNS; x++) {
            if (row.columns[x].packed != 0) {
                success |= SDL_SetRenderDrawColor(renderer,
                                                  0x55 * row.columns[x].r, // repeat the 2 bits by multiplying by 0x55
                                                  0x55 * row.columns[x].g,
                                                  0x55 * row.columns[x].b,
                                                  0x55 * row.columns[x].a);
                success |= SDL_RenderFillRect(renderer, &(SDL_Rect){x * BLOCK_WIDTH + 1, y * BLOCK_HEIGHT + 1, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1});
            }
        }
    }

    if (success < 0) {
        warn("\033[93mW\033[0m: something went wrong whilst renderering! SDL Error: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderPresent(renderer);
}
