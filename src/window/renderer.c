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
#include "colour8.h"
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
static inline int draw_block(SDL_Renderer* renderer, int8_t x, int8_t y) {
    return SDL_RenderFillRect(renderer, &(SDL_Rect){x * BLOCK_WIDTH + 1 + TET_PADDING, y * BLOCK_HEIGHT + 1 + TET_PADDING, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1});
}

// sets the colour32 from the colour8
static inline void set_colour(SDL_Renderer* renderer, Colour8 c) {
    (void)SDL_SetRenderDrawColor(renderer, colour8_red32(c), colour8_green32(c), colour8_blue32(c), 0xFF);
}

// draws a shape at the specified position
static void draw_shape(SDL_Renderer* const renderer, const ShapeId id, const int8_t pos_x, const int8_t pos_y) {
    const Shape shape = shape_from_id(id);
    set_colour(renderer, colour_from_id(id));

    for (int8_t y = 0; y < SHAPE_HEIGHT; y++) {
        const ShapeRow shape_row = shape_get_row(shape, y);

        if (shape_row == 0)
            continue;

        for (int8_t x = 0; x < SHAPE_WIDTH; x++)
            if (is_set(shape_row, x))
                draw_block(renderer, pos_x + x, pos_y + y);
    }
}

// draw the block data in the level
static void render_level(SDL_Renderer* renderer, GameData* data) {
    for (int8_t y = 0; y < ROWS; y++) {
        Row row = data->rows[y];

        for (int8_t x = 0; x < COLUMNS; x++) {
            if (row[x].packed != 0) {
                set_colour(renderer, row[x]);
                draw_block(renderer, x, y);
            }
        }
    }
}

void renderer_update(const RenderData* render_data) {
    SDL_Renderer* const renderer = render_data->renderer;
    const GameData* game_data = render_data->game_data;


    int success = 0; // if an error occurs, this value is <0

    // clear render
    set_colour(renderer, COLOUR_BLACK);
    success |= SDL_RenderClear(renderer);

    set_colour(renderer, COLOUR_WHITE);
    SDL_RenderDrawRect(renderer, &(SDL_Rect){TET_PADDING, TET_PADDING, TET_WIDTH + 1, TET_HEIGHT + 1});
    draw_shape(renderer, game_data->nxt[game_data->curr_idx + 1], COLUMNS + 1, 2); // draw the next shape

    render_level(renderer, render_data->game_data);
    draw_shape(renderer, game_data->nxt[game_data->curr_idx], game_data->sel_x, game_data->sel_y); // draw the current shape

    if (success < 0) {
        warn("something went wrong whilst renderering! SDL Error: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderPresent(renderer);
}
