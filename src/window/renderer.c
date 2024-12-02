// initializes the window and renderer
#include "renderer.h"

#include <SDL2/SDL.h>
#include <stdio.h>

#include "../errors.h"
#include "../main.h"


int renderer_init(SDL_Window** window, SDL_Renderer** renderer) {
    // create a new window
    *window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 100, 100, SDL_WINDOW_SHOWN);
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

    int success = 0; // if an error occurs, this value is <0

    // clear render
    success |= SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x50);
    success |= SDL_RenderClear(renderer);

    if (success < 0) {
        warn("\033[93mW\033[0m: something went wrong whilst renderering! SDL Error: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderPresent(renderer);
}
