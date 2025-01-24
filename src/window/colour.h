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


#define RED ((uint8_t)0b11000011)
#define YELLOW ((uint8_t)0b11110011)
#define ORANGE ((uint8_t)0b11100011)
#define GREEN ((uint8_t)0b00110011)
#define CYAN ((uint8_t)0b00111111)
#define BLUE ((uint8_t)0b00001111)
#define MAGENTA ((uint8_t)0b11001111)

#define NONE ((uint8_t)0)
#define BLACK ((uint8_t)3)
#define WHITE ((uint8_t)0xFF)


#define COLOUR_RED ((Colour){RED})
#define COLOUR_YELLOW ((Colour){YELLOW})
#define COLOUR_ORANGE ((Colour){ORANGE})
#define COLOUR_GREEN ((Colour){GREEN})
#define COLOUR_CYAN ((Colour){CYAN})
#define COLOUR_BLUE ((Colour){BLUE})
#define COLOUR_MAGENTA ((Colour){MAGENTA})

#define COLOUR_NONE ((Colour){NONE})
#define COLOUR_BLACK ((Colour){BLACK})
#define COLOUR_WHITE ((Colour){WHITE})
