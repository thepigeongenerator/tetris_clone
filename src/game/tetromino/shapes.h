#pragma once
#include <stdint.h>

#define SHAPE_WIDTH  4
#define SHAPE_HEIGHT 4

#define TETROMINO_COUNT 7
typedef uint16_t Shape;
enum {
    // clang-format off
    /*                              0    1    2    3         */
    TETROMINO_I     = 0x8888,    // 1000 1000 1000 1000      the I tetromino with no rotation
    TETROMINO_I_90  = 0x000F,    // 0000 0000 0000 1111      the I tetromino with a no rotation
    TETROMINO_O     = 0xCC00,    // 1100 1100 0000 0000      the O tetromino with no rotation
    TETROMINO_T     = 0xE400,    // 1110 0100 0000 0000      the T tetromino with no rotation
    TETROMINO_T_90  = 0xCC80,    // 1100 1100 1000 0000      the T tetromino with a no rotation
    TETROMINO_T_180 = 0x4E00,    // 0100 1110 0000 0000      the T tetromino with a 180° rotation
    TETROMINO_T_270 = 0x2620,    // 0010 0110 0010 0000      the T tetromino with a 270° rotation
    TETROMINO_L     = 0x88C0,    // 1000 1000 1100 0000      the L tetromino with no rotation
    TETROMINO_L_90  = 0xE800,    // 1110 1000 0000 0000      the L tetromino with a no rotation
    TETROMINO_L_180 = 0xC440,    // 1100 0100 0100 0000      the L tetromino with a 180° rotation
    TETROMINO_L_270 = 0x02E0,    // 0000 0010 1110 0000      the L tetromino with a 270° rotation
    TETROMINO_J     = 0x44C0,    // 0100 0100 1100 0000      the J tetromino with no rotation
    TETROMINO_J_90  = 0x8E00,    // 1000 1110 0000 0000      the J tetromino with a no rotation
    TETROMINO_J_180 = 0xC880,    // 1100 1000 1000 0000      the J tetromino with a 180° rotation
    TETROMINO_J_270 = 0xC400,    // 1110 0010 0000 0000      the J tetromino with a 270° rotation
    TETROMINO_S     = 0x6C00,    // 0110 1100 0000 0000      the S tetromino with no rotation
    TETROMINO_S_90  = 0x8C40,    // 1000 1100 0100 0000      the S tetromino with a no rotation
    TETROMINO_Z     = 0xC600,    // 1100 0110 0000 0000      the Z tetromino with no rotation
    TETROMINO_Z_90  = 0x4C80,    // 0100 1100 1000 0000      the Z tetromino with a no rotation
    // clang-format on
};
typedef uint8_t ShapeRow;

static inline ShapeRow shape_get_row(Shape shape, uint8_t index) {
    return shape >> (((SHAPE_HEIGHT - 1) - index) * 4) & 0xF;
}

static inline _Bool is_set(ShapeRow row, uint8_t index) {
    return (row >> ((SHAPE_WIDTH - 1) - index) & 1) != 0;
}
