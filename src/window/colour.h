#pragma once

// fits colours with each channel having a width of 2 bits
#include <stdint.h>

typedef union {
    uint8_t packed;
    struct {
        uint8_t a : 2;
        uint8_t b : 2;
        uint8_t g : 2;
        uint8_t r : 2;
    };
} Colour;

#define COLOUR_RED      ((Colour){0b11000011})
#define COLOUR_YELLOW   ((Colour){0b11110011})
#define COLOUR_ORANGE   ((Colour){0b11100011})
#define COLOUR_GREEN    ((Colour){0b00110011})
#define COLOUR_CYAN     ((Colour){0b00111111})
#define COLOUR_BLUE     ((Colour){0b00001111})
#define COLOUR_MAGENTA  ((Colour){0b11001111})
