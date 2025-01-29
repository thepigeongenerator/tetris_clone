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
#include "../game/tetromino/shapes.h"
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

// draws a block at the specified position
static inline int draw_block(SDL_Renderer* renderer, uint8_t x, uint8_t y) {
    return SDL_RenderFillRect(renderer, &(SDL_Rect){x * BLOCK_WIDTH + 1, y * BLOCK_HEIGHT + 1, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1});
}

// sets the colour32 from the colour8
static inline void set_colour(SDL_Renderer* renderer, Colour c) {
    (void)SDL_SetRenderDrawColor(renderer, colour_red32(c), colour_green32(c), colour_blue32(c), colour_alpha32(c));
}

// draw the selected block
static void render_selected(SDL_Renderer* renderer, SelectedShape selected) {
    Shape selected_shape = shape_from_id(selected.id);
    set_colour(renderer, colour_from_id(selected.id));

    for (uint8_t y = 0; y < SHAPE_HEIGHT; y++) {
        ShapeRow shape_row = shape_get_row(selected_shape, y);

        if (shape_row == 0)
            continue;

        for (uint8_t x = 0; x < SHAPE_WIDTH; x++)
            if (is_set(shape_row, x))
                draw_block(renderer, selected.x + x, selected.y + y);
    }
}

// draw the block data in the level
static void render_level(SDL_Renderer* renderer, GameData* data) {
    for (uint8_t y = 0; y < ROWS; y++) {
        Row row = data->row[y];

        for (uint8_t x = 0; x < COLUMNS; x++) {
            if (row[x].packed != 0) {
                set_colour(renderer, row[x]);
                draw_block(renderer, x, y);
            }
        }
    }
}

void renderer_update(const RenderData* render_data) {
    SDL_Renderer* renderer = render_data->renderer;

    int success = 0; // if an error occurs, this value is <0

    // clear render
    success |= SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    success |= SDL_RenderClear(renderer);

    render_selected(renderer, render_data->game_data->selected);
    render_level(renderer, render_data->game_data);

    if (success < 0) {
        warn("something went wrong whilst renderering! SDL Error: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderPresent(renderer);
}
