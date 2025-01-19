// initializes the window and renderer
#include "renderer.h"

#include <SDL2/SDL.h>
#include <stdio.h>

#include "../errors.h"
#include "SDL_render.h"


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

static inline int set_block(SDL_Renderer* renderer, const Row row, const uint8_t block, const uint8_t filter, const uint8_t x, const uint8_t y) {
    uint8_t colour = block & filter;
    return SDL_SetRenderDrawColor(renderer,
                                  0xFF * (0 != (colour & (RED | RED << 3))),
                                  0xFF * (0 != (colour & (GREEN | GREEN << 3))),
                                  0xFF * (0 != (colour & (BLUE | BLUE << 3))),
                                  0xFF) |
           SDL_RenderFillRect(renderer, &(SDL_Rect){x * BLOCK_WIDTH + 1, y * BLOCK_HEIGHT + 1, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1});
}

void renderer_update(const RenderData* render_data) {
    SDL_Renderer* renderer = render_data->renderer;
    GameData* data = render_data->game_data;

    int success = 0; // if an error occurs, this value is <0

    // clear render
    success |= SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x50);
    success |= SDL_RenderClear(renderer);

    for (uint8_t y = 0; y < ROWS; y++) {
        Row row = data->row[y];

        for (uint8_t x = 0; x < COLUMNS; x += 2) {
            uint8_t block = row >> (x * 3); // get the two blocks stored at this column

            if ((block & 0x0F) != 0)
                success |= set_block(renderer, row, block, 0x0F, x + 1, y);

            if ((block & 0xF0) != 0)
                success |= set_block(renderer, row, block, 0xF0, x, y);
        }
    }

    if (success < 0) {
        warn("\033[93mW\033[0m: something went wrong whilst renderering! SDL Error: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderPresent(renderer);
}
