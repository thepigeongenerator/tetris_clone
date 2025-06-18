#pragma once
#include <stdint.h>

#include "SDL_render.h"

// stores colour in a rgba format, each channel being a 8 bits wide.
typedef union {
	uint32_t packed;
	struct {
		uint8_t a;
		uint8_t b;
		uint8_t g;
		uint8_t r;
	};
} colour32;

#define COLOUR32_BLACK   ((colour32){0x000000FF})
#define COLOUR32_RED     ((colour32){0xFF0000FF})
#define COLOUR32_YELLOW  ((colour32){0xFFFF00FF})
#define COLOUR32_ORANGE  ((colour32){0xFF6D00FF})
#define COLOUR32_GREEN   ((colour32){0x00FF00FF})
#define COLOUR32_CYAN    ((colour32){0x00FFFFFF})
#define COLOUR32_BLUE    ((colour32){0x0000FFFF})
#define COLOUR32_MAGENTA ((colour32){0xFF00FFFF})
#define COLOUR32_WHITE   ((colour32){0xFFFFFFFF})

// sets the render colour to a colour32 value
static inline void set_colour32(SDL_Renderer* const renderer, colour32 const c) {
	(void)SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

// american macros:
#define color32         colour32
#define COLOR32_BLACK   COLOUR32_BLACK
#define COLOR32_RED     COLOUR32_RED
#define COLOR32_YELLOW  COLOUR32_YELLOW
#define COLOR32_ORANGE  COLOUR32_ORANGE
#define COLOR32_GREEN   COLOUR32_GREEN
#define COLOR32_CYAN    COLOUR32_CYAN
#define COLOR32_BLUE    COLOUR32_BLUE
#define COLOR32_MAGENTA COLOUR32_MAGENTA
#define COLOR32_WHITE   COLOUR32_WHITE
