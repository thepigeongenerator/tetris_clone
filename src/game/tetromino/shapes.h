#pragma once
#include <stdint.h>

#include "../../window/colour.h"

typedef uint16_t Shape;
typedef uint8_t ShapeRow;

typedef uint8_t ShapeId;
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

#define SHAPE_WIDTH     ((uint8_t)(sizeof(Shape) * 8 / 2))
#define SHAPE_HEIGHT    ((uint8_t)(sizeof(Shape) * 8 / 2))
#define TETROMINO_COUNT 7



static inline ShapeRow shape_get_row(Shape shape, uint8_t index) {
    return shape >> (((SHAPE_HEIGHT - 1) - index) * 4) & 0xF;
}

static inline _Bool is_set(ShapeRow row, uint8_t index) {
    return (row >> ((SHAPE_WIDTH - 1) - index) & 1) != 0;
}

Shape shape_from_id(ShapeId id);
Colour colour_from_id(ShapeId id);
