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
#include <stdlib.h>

#include "../errors.h"
#include "../game/game.h"
#include "../game/tetromino/shapes.h"
#include "SDL_surface.h"
#include "colour8.h"
#include "renderer.h"

#define COLOUR_SCORE COLOUR_YELLOW

void renderer_init(render_data* const render_dat, game_data const* const game_dat) {
    SDL_Window* const window = SDL_CreateWindow("tetris clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) error(ERROR_SDL_RENDERER_INIT, "Window failed to be created! SDL Error: %s", SDL_GetError());

    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) error(ERROR_SDL_RENDERER_INIT, "Renderer failed to be created! SDL Error: %s", SDL_GetError());

    TTF_Font* const font = TTF_OpenFont("pixeldroid_botic-regular.ttf", PX_DENS);
    if (font == NULL) error(ERROR_SDL_FONT_INIT, "Failed to open font! TTF Error: %s", TTF_GetError());

    // initialize the render data
    *render_dat = (render_data){
        game_dat,
        window,
        renderer,
        font,
        calloc(1, sizeof(struct render_cache)), // zero-initialize the memory as we read from it
    };
}

static inline int32_t get_column_pos(uint8_t column) {
    return column * BLOCK_WIDTH + 1 + TET_PADDING;
}

static inline int32_t get_row_pos(uint8_t row) {
    return row * BLOCK_HEIGHT + 1 + TET_PADDING;
}

static void draw_score_text(render_data const* dat) {
    struct render_cache* const cache = dat->cache;
    uint16_t const score = dat->game_dat->score;

    SDL_Renderer* const renderer = dat->renderer;
    TTF_Font* const font = dat->font;

    if (!(cache->prevscore != score && cache->score_texture != NULL)) {
        char score_text[6]; // max digits of a uint16 + \0 terminator
        if (!score) sprintf(score_text, "0");
        else sprintf(score_text, "%hu0", score);

        SDL_Surface* const txt_surface = TTF_RenderText_Solid(font, score_text, (SDL_Colour){colour8_red32(COLOUR_SCORE), colour8_green32(COLOUR_SCORE), colour8_blue32(COLOUR_SCORE), 0xFF});
        SDL_Texture* const txt_texture = SDL_CreateTextureFromSurface(renderer, txt_surface);

        if (cache->score_texture != NULL) {
            // free old data
            SDL_FreeSurface(cache->score_surface);
            SDL_DestroyTexture(cache->score_texture);
        }

        // write data to cache
        cache->score_surface = txt_surface;
        cache->score_texture = txt_texture;
    }

    SDL_Rect text_rect = {get_column_pos(COLUMNS + 1), get_row_pos(0), cache->score_surface->w, cache->score_surface->h};
    SDL_RenderCopy(renderer, cache->score_texture, NULL, &text_rect);
}

// draws a block at the specified position
static inline int draw_block(SDL_Renderer* const renderer, int8_t const x, int8_t const y) {
    SDL_Rect const block = {get_column_pos(x), get_row_pos(y), BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1};
    return SDL_RenderFillRect(renderer, &block);
}

// sets the colour32 from the colour8
static inline void set_colour(SDL_Renderer* const renderer, colour8 const c) {
    (void)SDL_SetRenderDrawColor(renderer, colour8_red32(c), colour8_green32(c), colour8_blue32(c), 0xFF);
}

// draws a shape at the specified position
static void draw_shape(SDL_Renderer* const renderer, shape_id const id, int8_t const pos_x, int8_t const pos_y) {
    shape const shape = shape_from_id(id);
    set_colour(renderer, colour_from_id(id));

    for (int8_t y = 0; y < SHAPE_HEIGHT; y++) {
        shape_row const shape_row = shape_get_row(shape, y);

        if (shape_row == 0)
            continue;

        for (int8_t x = 0; x < SHAPE_WIDTH; x++)
            if (shape_is_set(shape_row, x))
                draw_block(renderer, pos_x + x, pos_y + y);
    }
}

// draw the block data in the level
static void render_level(SDL_Renderer* const renderer, game_data const* const data) {
    for (int8_t y = 0; y < ROWS; y++) {
        row_const const row = data->rows[y];

        for (int8_t x = 0; x < COLUMNS; x++) {
            if (row[x] != 0) {
                set_colour(renderer, row[x]);
                draw_block(renderer, x, y);
            }
        }
    }
}

void renderer_update(render_data const* const dat) {
    SDL_Renderer* const renderer = dat->renderer;
    game_data const* const game_data = dat->game_dat;


    int success = 0; // if an error occurs, this value is <0

    // clear render
    set_colour(renderer, COLOUR_BLACK);
    success |= SDL_RenderClear(renderer);

    set_colour(renderer, COLOUR_WHITE);

    static SDL_Rect const field_size = {TET_PADDING, TET_PADDING, TET_WIDTH + 1, TET_HEIGHT + 1};
    SDL_RenderDrawRect(renderer, &field_size);
    draw_shape(renderer, game_data->nxt[game_data->curr_idx + 1], COLUMNS + 1, 3); // draw the next shape

    draw_score_text(dat);

    render_level(renderer, dat->game_dat);
    draw_shape(renderer, game_data->nxt[game_data->curr_idx], game_data->sel_x, game_data->sel_y); // draw the current shape

    if (success < 0) {
        warn("something went wrong whilst renderering! SDL Error: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderPresent(renderer);
}

void renderer_free(render_data* const render_data) {
    SDL_DestroyRenderer(render_data->renderer);
    SDL_DestroyWindow(render_data->window);
    TTF_CloseFont(render_data->font);
    free(render_data->cache);
    render_data->renderer = NULL;
    render_data->window = NULL;
    render_data->font = NULL;
    render_data->cache = NULL;
}
