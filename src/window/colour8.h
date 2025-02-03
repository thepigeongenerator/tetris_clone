#pragma once
#include <stdint.h>

// fits colours with each channel having a width of 2 bits
typedef union {
    uint8_t packed;
    struct {
        uint8_t b : 2;
        uint8_t g : 3;
        uint8_t r : 3;
    };
} Colour8;

/*                                          rrrg ggbb */
#define COLOUR_BLACK   ((Colour8){0x00}) // 0000 0000
#define COLOUR_RED     ((Colour8){0xE0}) // 1110 0000
#define COLOUR_YELLOW  ((Colour8){0xFC}) // 1111 1100
#define COLOUR_ORANGE  ((Colour8){0xEC}) // 1111 1100
#define COLOUR_GREEN   ((Colour8){0x1C}) // 0001 1100
#define COLOUR_CYAN    ((Colour8){0x1F}) // 0001 1111
#define COLOUR_BLUE    ((Colour8){0x03}) // 0000 0011
#define COLOUR_MAGENTA ((Colour8){0xE3}) // 1110 0011
#define COLOUR_WHITE   ((Colour8){0xFF}) // 1111 1111

// gets the red channel in 32 bit colour space
static inline uint8_t colour8_red32(Colour8 colour) {
    return colour.r * (255 / 7);
}

// gets the green channel in 32 bit colour space
static inline uint8_t colour8_green32(Colour8 colour) {
    return colour.g * (255 / 7);
}

// gets the blue channel in 32 bit colour space
static inline uint8_t colour8_blue32(Colour8 colour) {
    return colour.b * (255 / 3);
}
