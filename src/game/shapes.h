#include <stdint.h>
#define SHAPE_WIDTH  4
#define SHAPE_HEIGHT 4


typedef enum : uint16_t {
    // clang-format off
    /*                                          0    1    2    3         */
    TETROMINO_I     = 0b1000100010001000,    // 1000 1000 1000 1000      the I tetromino with no rotation
    TETROMINO_I_90  = 0b0000000000001111,    // 0000 0000 0000 1111      the I tetromino with a no rotation
    TETROMINO_O     = 0b1100110000000000,    // 1100 1100 0000 0000      the O tetromino with no rotation
    TETROMINO_T     = 0b1110010000000000,    // 1110 0100 0000 0000      the T tetromino with no rotation
    TETROMINO_T_90  = 0b1100110010000000,    // 1100 1100 1000 0000      the T tetromino with a no rotation
    TETROMINO_T_180 = 0b0100111000000000,    // 0100 1110 0000 0000      the T tetromino with a 180° rotation
    TETROMINO_T_270 = 0b0010011000100000,    // 0010 0110 0010 0000      the T tetromino with a 270° rotation
    TETROMINO_L     = 0b1000100011000000,    // 1000 1000 1100 0000      the L tetromino with no rotation
    TETROMINO_L_90  = 0b1110100000000000,    // 1110 1000 0000 0000      the L tetromino with a no rotation
    TETROMINO_L_180 = 0b1100010001000000,    // 1100 0100 0100 0000      the L tetromino with a 180° rotation
    TETROMINO_L_270 = 0b0000001011100000,    // 0000 0010 1110 0000      the L tetromino with a 270° rotation
    TETROMINO_J     = 0b0100010011000000,    // 0100 0100 1100 0000      the J tetromino with no rotation
    TETROMINO_J_90  = 0b1000111000000000,    // 1000 1110 0000 0000      the J tetromino with a no rotation
    TETROMINO_J_180 = 0b1100100010000000,    // 1100 1000 1000 0000      the J tetromino with a 180° rotation
    TETROMINO_J_270 = 0b1110001000000000,    // 1110 0010 0000 0000      the J tetromino with a 270° rotation
    TETROMINO_S     = 0b0110110000000000,    // 0110 1100 0000 0000      the S tetromino with no rotation
    TETROMINO_S_90  = 0b1000110001000000,    // 1000 1100 0100 0000      the S tetromino with a no rotation
    TETROMINO_Z     = 0b1100011000000000,    // 1100 0110 0000 0000      the Z tetromino with no rotation
    TETROMINO_Z_90  = 0b0100110010000000,    // 0100 1100 1000 0000      the Z tetromino with a no rotation
    // clang-format on
} Shape;
typedef uint8_t ShapeRow;

static inline ShapeRow shape_get_row(Shape shape, uint8_t index) {
    return shape >> (((SHAPE_HEIGHT - 1) - index) * 4) & 0b1111;
}

static inline _Bool is_set(ShapeRow row, uint8_t index) {
    return (row >> ((SHAPE_WIDTH - 1) - index) & 1) != 0;
}
