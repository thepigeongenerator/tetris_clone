#include "shapes.h"

/*                                          0    1    2    3         */
#define TET_SHAPE_O     ((Shape)0xCC00) // 1100 1100 0000 0000      the O tetromino with no rotation
#define TET_SHAPE_I     ((Shape)0x8888) // 1000 1000 1000 1000      the I tetromino with no rotation
#define TET_SHAPE_I_90  ((Shape)0x000F) // 0000 0000 0000 1111      the I tetromino with a no rotation
#define TET_SHAPE_S     ((Shape)0x6C00) // 0110 1100 0000 0000      the S tetromino with no rotation
#define TET_SHAPE_S_90  ((Shape)0x8C40) // 1000 1100 0100 0000      the S tetromino with a no rotation
#define TET_SHAPE_Z     ((Shape)0xC600) // 1100 0110 0000 0000      the Z tetromino with no rotation
#define TET_SHAPE_Z_90  ((Shape)0x4C80) // 0100 1100 1000 0000      the Z tetromino with a no rotation
#define TET_SHAPE_T     ((Shape)0xE400) // 1110 0100 0000 0000      the T tetromino with no rotation
#define TET_SHAPE_T_90  ((Shape)0xCC80) // 1100 1100 1000 0000      the T tetromino with a no rotation
#define TET_SHAPE_T_180 ((Shape)0x4E00) // 0100 1110 0000 0000      the T tetromino with a 180° rotation
#define TET_SHAPE_T_270 ((Shape)0x2620) // 0010 0110 0010 0000      the T tetromino with a 270° rotation
#define TET_SHAPE_L     ((Shape)0x88C0) // 1000 1000 1100 0000      the L tetromino with no rotation
#define TET_SHAPE_L_90  ((Shape)0xE800) // 1110 1000 0000 0000      the L tetromino with a no rotation
#define TET_SHAPE_L_180 ((Shape)0xC440) // 1100 0100 0100 0000      the L tetromino with a 180° rotation
#define TET_SHAPE_L_270 ((Shape)0x02E0) // 0000 0010 1110 0000      the L tetromino with a 270° rotation
#define TET_SHAPE_J     ((Shape)0x44C0) // 0100 0100 1100 0000      the J tetromino with no rotation
#define TET_SHAPE_J_90  ((Shape)0x8E00) // 1000 1110 0000 0000      the J tetromino with a no rotation
#define TET_SHAPE_J_180 ((Shape)0xC880) // 1100 1000 1000 0000      the J tetromino with a 180° rotation
#define TET_SHAPE_J_270 ((Shape)0xC400) // 1110 0010 0000 0000      the J tetromino with a 270° rotation

Shape shape_from_id(ShapeId id) {
    // clang-format off
    static const Shape shapes[TETROMINO_COUNT][4] = {
        // 0°         90°             180°             170°
        {TET_SHAPE_O, TET_SHAPE_O,    TET_SHAPE_O,     TET_SHAPE_O},
        {TET_SHAPE_I, TET_SHAPE_I_90, TET_SHAPE_I,     TET_SHAPE_I_90},
        {TET_SHAPE_S, TET_SHAPE_S_90, TET_SHAPE_S,     TET_SHAPE_S_90},
        {TET_SHAPE_Z, TET_SHAPE_Z_90, TET_SHAPE_Z,     TET_SHAPE_Z_90},
        {TET_SHAPE_T, TET_SHAPE_T_90, TET_SHAPE_T,     TET_SHAPE_T_90},
        {TET_SHAPE_L, TET_SHAPE_L_90, TET_SHAPE_L_180, TET_SHAPE_L_270},
        {TET_SHAPE_J, TET_SHAPE_J_90, TET_SHAPE_J_180, TET_SHAPE_J_270},
    };
    // clang-format on

    // first 3 bits is the shape type, the rest is rotation data
    return shapes[id & 7][id >> 3];
}
