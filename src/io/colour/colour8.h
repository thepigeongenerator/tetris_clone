#pragma once
#include <stdint.h>

#include "SDL_render.h"

// stores colour in a rrrgggbb format, which maps exactly to 8 bits
typedef uint8_t colour8;

/*                                         rrrg ggbb */
#define COLOUR8_BLACK   ((colour8)0x00) // 0000 0000
#define COLOUR8_RED     ((colour8)0xE0) // 1110 0000
#define COLOUR8_YELLOW  ((colour8)0xFC) // 1111 1100
#define COLOUR8_ORANGE  ((colour8)0xEC) // 1111 1100
#define COLOUR8_GREEN   ((colour8)0x1C) // 0001 1100
#define COLOUR8_CYAN    ((colour8)0x1F) // 0001 1111
#define COLOUR8_BLUE    ((colour8)0x03) // 0000 0011
#define COLOUR8_MAGENTA ((colour8)0xE3) // 1110 0011
#define COLOUR8_WHITE   ((colour8)0xFF) // 1111 1111

// gets the red channel in 32 bit colour space
static inline uint8_t colour8_red32(colour8 const colour) {
    return (colour >> 5) * (255 / 7);
}

// gets the green channel in 32 bit colour space
static inline uint8_t colour8_green32(colour8 const colour) {
    return ((colour >> 2) & 7) * (255 / 7);
}

// gets the blue channel in 32 bit colour space
static inline uint8_t colour8_blue32(colour8 const colour) {
    return (colour & 3) * (255 / 3);
}

// sets the render colour to a colour8 value
static inline void set_colour8(SDL_Renderer* const renderer, colour8 const c) {
    (void)SDL_SetRenderDrawColor(renderer, colour8_red32(c), colour8_green32(c), colour8_blue32(c), 0xFF);
}

// american macros:
#define color8         colour8
#define color8_red32   colour8_red32
#define color8_green32 colour8_green32
#define color8_blue32  colour8_blue32
#define COLOR8_BLACK   COLOUR8_BLACK
#define COLOR8_RED     COLOUR8_RED
#define COLOR8_YELLOW  COLOUR8_YELLOW
#define COLOR8_ORANGE  COLOUR8_ORANGE
#define COLOR8_GREEN   COLOUR8_GREEN
#define COLOR8_CYAN    COLOUR8_CYAN
#define COLOR8_BLUE    COLOUR8_BLUE
#define COLOR8_MAGENTA COLOUR8_MAGENTA
#define COLOR8_WHITE   COLOUR8_WHITE
