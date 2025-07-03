#include "render.h"

#include <SDL_error.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <SDL_video.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "../error.h"
#include "../game/game.h"
#include "../game/tetromino/shapes.h"
#include "../util/types.h"
#include "../util/vec.h"
#include "colour/colour32.h"
#include "colour/colour8.h"

#define COLOUR_SCORE COLOUR32_YELLOW

SDL_Renderer* rend = NULL;
TTF_Font* font = NULL;
struct gamedata const* gdat = NULL;

static SDL_Surface* score_surface = NULL;
static SDL_Texture* score_texture = NULL;

void render_init(SDL_Window* win, struct gamedata const* game_data) {
	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (rend == NULL) fatal(ERROR_SDL_RENDERING_INIT, "Renderer failed to be created! SDL Error: %s", SDL_GetError());

	font = TTF_OpenFont("pixeldroid_botic-regular.ttf", PX_DENS);
	if (font == NULL) error("Failed to open font! TTF Error: %s", TTF_GetError());

	gdat = game_data;
}

static inline int32_t get_column_pos(uint column) {
	return column * BLOCK_WIDTH + 1 + TET_PADDING;
}

static inline int32_t get_row_pos(uint row) {
	return row * BLOCK_HEIGHT + 1 + TET_PADDING;
}

static void draw_score_text(void) {
	static u16 prev_pts = 0xFFFF;

	if (prev_pts ^ gdat->pnts) {
		char score_text[6] = "0"; // max digits of a u16 + null terminator
		prev_pts = gdat->pnts;
		if (gdat->pnts) sprintf(score_text, "%hu0", gdat->pnts);

		SDL_FreeSurface(score_surface);
		SDL_DestroyTexture(score_texture);
		score_surface = TTF_RenderText_Solid(font, score_text, (SDL_Colour){COLOUR_SCORE.r, COLOUR_SCORE.g, COLOUR_SCORE.b, COLOUR_SCORE.a});
		score_texture = SDL_CreateTextureFromSurface(rend, score_surface);
	}

	assert(score_surface && score_texture);
	SDL_Rect text_rect = {get_column_pos(COLUMNS + 1), get_row_pos(0), score_surface->w, score_surface->h};
	SDL_RenderCopy(rend, score_texture, NULL, &text_rect);
}

// TODO: this is suboptimal, since each block will be 2 triangles, wasting perf. Consider using switch...case hard-coded drawing
// draws a block at the specified position
static inline int draw_block(SDL_Renderer* const renderer, i8vec2 pos) {
	SDL_Rect const block = {get_column_pos(pos[VX]), get_row_pos(pos[VY]), BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1};
	return SDL_RenderFillRect(renderer, &block);
}

// draws a shape at the specified position
static void draw_shape(u8 const id, i8vec2 pos) {
	set_colour8(rend, colour_from_id(id));
	i8vec2 bpos[4];
	shape_getblocks(id, bpos);
	draw_block(rend, pos + bpos[0]);
	draw_block(rend, pos + bpos[1]);
	draw_block(rend, pos + bpos[2]);
	draw_block(rend, pos + bpos[3]);
}

// draw the block data in the level
static void render_level(void) {
	for (int y = 0; y < ROWS; y++) {
		u8 const* row = gdat->rows[y];

		for (int x = 0; x < COLUMNS; x++) {
			if (row[x] != 0) {
				set_colour8(rend, row[x]);
				draw_block(rend, (i8vec2){x, y});
			}
		}
	}
}

void render_update(void) {
	set_colour32(rend, COLOUR32_BLACK);
	SDL_RenderClear(rend);
	set_colour32(rend, COLOUR32_WHITE);

	static SDL_Rect const field_size = {TET_PADDING, TET_PADDING, TET_WIDTH + 1, TET_HEIGHT + 1};
	SDL_RenderDrawRect(rend, &field_size);

	if (font) draw_score_text();

	render_level();
	draw_shape(gdat->pdat.cur, gdat->pdat.sel);
	draw_shape(gdat->pdat.nxt[gdat->pdat.idx], (i8vec2){COLUMNS + 1, 3});

	SDL_RenderPresent(rend);
}

void render_free(void) {
	assert(rend);
	SDL_DestroyRenderer(rend);
	rend = NULL;

	TTF_CloseFont(font);
	SDL_FreeSurface(score_surface);
	SDL_DestroyTexture(score_texture);
	font = NULL;
	score_surface = NULL;
	score_texture = NULL;
}
