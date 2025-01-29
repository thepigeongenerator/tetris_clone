#pragma once
#include <stdint.h>

// fits colours with each channel having a width of 2 bits
typedef union {
    uint8_t packed;
    struct {
        uint8_t a : 2;
        uint8_t b : 2;
        uint8_t g : 2;
        uint8_t r : 2;
    };
} Colour;


#define NONE    ((uint8_t)0)
#define BLACK   ((uint8_t)3)
#define RED     ((uint8_t)0xC0 | BLACK)
#define YELLOW  ((uint8_t)0xF0 | BLACK)
#define ORANGE  ((uint8_t)0xE0 | BLACK)
#define GREEN   ((uint8_t)0x30 | BLACK)
#define CYAN    ((uint8_t)0x3C | BLACK)
#define BLUE    ((uint8_t)0x0C | BLACK)
#define MAGENTA ((uint8_t)0xCC | BLACK)
#define WHITE   ((uint8_t)0xFF)

#define COLOUR_NONE    ((Colour){NONE})
#define COLOUR_BLACK   ((Colour){BLACK})
#define COLOUR_RED     ((Colour){RED})
#define COLOUR_YELLOW  ((Colour){YELLOW})
#define COLOUR_ORANGE  ((Colour){ORANGE})
#define COLOUR_GREEN   ((Colour){GREEN})
#define COLOUR_CYAN    ((Colour){CYAN})
#define COLOUR_BLUE    ((Colour){BLUE})
#define COLOUR_MAGENTA ((Colour){MAGENTA})
#define COLOUR_WHITE   ((Colour){WHITE})

// gets the red channel in 32 bit colour space
static inline uint8_t colour_red32(Colour colour) {
    return colour.r * 0x55;
}

// gets the green channel in 32 bit colour space
static inline uint8_t colour_green32(Colour colour) {
    return colour.g * 0x55;
}

// gets the blue channel in 32 bit colour space
static inline uint8_t colour_blue32(Colour colour) {
    return colour.b * 0x55;
}

// gets the alpha channel in 32 bit colour space
static inline uint8_t colour_alpha32(Colour colour) {
    return colour.a * 0x55;
}

// gets the packed colour in 32 bit colour space
static inline uint32_t colour_packed32(Colour colour) {
    return colour.packed * 0x55;
}
