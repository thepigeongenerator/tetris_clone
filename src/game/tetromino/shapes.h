#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "../../window/colour8.h"

typedef uint16_t shape;
typedef uint8_t shape_row;

typedef uint8_t shape_id;
enum {
    TETROMINO_O = 0,
    TETROMINO_I = 1,
    TETROMINO_S = 2,
    TETROMINO_Z = 3,
    TETROMINO_T = 4,
    TETROMINO_L = 5,
    TETROMINO_J = 6,
    TETROMINO_ROTATED_90 = 8,
    TETROMINO_ROTATED_180 = 16,
    TETROMINO_ROTATED_270 = 24,
};

#define SHAPE_WIDTH     4
#define SHAPE_HEIGHT    4
#define TETROMINO_COUNT 7



static inline shape_row shape_get_row(shape const shape, uint8_t const index) {
    return shape >> (((SHAPE_HEIGHT - 1) - index) * SHAPE_WIDTH) & 0xF;
}

static inline bool shape_is_set(shape_row const row, uint8_t const index) {
    return (row >> ((SHAPE_WIDTH - 1) - index) & 1) != 0;
}

shape shape_from_id(shape_id id);
colour8 colour_from_id(shape_id id);
