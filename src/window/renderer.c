// initializes the window and renderer
#include "renderer.h"

#include <SDL_error.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <SDL_video.h>
#include <stdint.h>
#include <stdio.h>

#include "../errors.h"
#include "../game/game.h"
#include "../game/tetromino/shapes.h"
#include "SDL_surface.h"
#include "colour8.h"
#include "renderer.h"


void renderer_init(RenderData* const render_data, GameData const* const game_data) {
    SDL_Window* const window = SDL_CreateWindow("tetris clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) error(ERROR_SDL_RENDERER_INIT, "Window failed to be created! SDL Error: %s", SDL_GetError());

    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) error(ERROR_SDL_RENDERER_INIT, "Renderer failed to be created! SDL Error: %s", SDL_GetError());

    TTF_Font* const font = TTF_OpenFont("/usr/share/fonts/TTF/JetBrainsMono-Regular.ttf", PX_DENS);
    if (font == NULL) error(ERROR_SDL_FONT_INIT, "Failed to open font! TTF Error: %s", TTF_GetError());

    // initialize the render data
    *render_data = (RenderData){
        game_data,
        window,
        renderer,
        font};
}

// draws a block at the specified position
static inline int draw_block(SDL_Renderer* const renderer, int8_t const x, int8_t const y) {
    SDL_Rect const block = {x * BLOCK_WIDTH + 1 + TET_PADDING, y * BLOCK_HEIGHT + 1 + TET_PADDING, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1};
    return SDL_RenderFillRect(renderer, &block);
}

// sets the colour32 from the colour8
static inline void set_colour(SDL_Renderer* const renderer, Colour8 const c) {
    (void)SDL_SetRenderDrawColor(renderer, colour8_red32(c), colour8_green32(c), colour8_blue32(c), 0xFF);
}

// draws a shape at the specified position
static void draw_shape(SDL_Renderer* const renderer, ShapeId const id, int8_t const pos_x, int8_t const pos_y) {
    Shape const shape = shape_from_id(id);
    set_colour(renderer, colour_from_id(id));

    for (int8_t y = 0; y < SHAPE_HEIGHT; y++) {
        ShapeRow const shape_row = shape_get_row(shape, y);

        if (shape_row == 0)
            continue;

        for (int8_t x = 0; x < SHAPE_WIDTH; x++)
            if (is_set(shape_row, x))
                draw_block(renderer, pos_x + x, pos_y + y);
    }
}

// draw the block data in the level
static void render_level(SDL_Renderer* const renderer, GameData const* const data) {
    for (int8_t y = 0; y < ROWS; y++) {
        CRow const row = data->rows[y];

        for (int8_t x = 0; x < COLUMNS; x++) {
            if (row[x].packed != 0) {
                set_colour(renderer, row[x]);
                draw_block(renderer, x, y);
            }
        }
    }
}

void renderer_update(RenderData const* const render_data) {
    SDL_Renderer* const renderer = render_data->renderer;
    GameData const* const game_data = render_data->game_data;


    int success = 0; // if an error occurs, this value is <0

    // clear render
    set_colour(renderer, COLOUR_BLACK);
    success |= SDL_RenderClear(renderer);

    set_colour(renderer, COLOUR_WHITE);

    static SDL_Rect const field_size = {TET_PADDING, TET_PADDING, TET_WIDTH + 1, TET_HEIGHT + 1};
    SDL_RenderDrawRect(renderer, &field_size);
    draw_shape(renderer, game_data->nxt[game_data->curr_idx + 1], COLUMNS + 1, 3); // draw the next shape
    SDL_Surface* const surface = TTF_RenderText_Solid(render_data->font, "Hello, World!", (SDL_Colour){0xFF, 0xFF, 0xFF, 0xFF});
    SDL_Texture* const texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &(SDL_Rect){10, 10, surface->w, surface->h});
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    render_level(renderer, render_data->game_data);
    draw_shape(renderer, game_data->nxt[game_data->curr_idx], game_data->sel_x, game_data->sel_y); // draw the current shape

    if (success < 0) {
        warn("something went wrong whilst renderering! SDL Error: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderPresent(renderer);
}

void renderer_free(RenderData* const render_data) {
    SDL_DestroyRenderer(render_data->renderer);
    SDL_DestroyWindow(render_data->window);
    TTF_CloseFont(render_data->font);
    render_data->renderer = NULL;
    render_data->window = NULL;
    render_data->font = NULL;
}
