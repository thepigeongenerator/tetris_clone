#include "shapes.h"

#include "../../window/colour/colour8.h"
#include "../opts.h"

/*                                     0    1    2    3         */
#define SHAPE_O     ((shape)0x0660) // 0000 0110 0110 0000      the O tetromino with no rotation

#define SHAPE_I     ((shape)0x0F00) // 0000 1111 0000 0000      the I tetromino with no rotation
#define SHAPE_I_90  ((shape)0x2222) // 0010 0010 0010 0010      the I tetromino with a 90° rotation
#define SHAPE_I_180 ((shape)0x00F0) // 0000 0000 1111 0000      the I tetromino with a 180° rotation
#define SHAPE_I_270 ((shape)0x4444) // 0100 0100 0100 0100      the I tetromino with a 270° rotation

#define SHAPE_S     ((shape)0xC600) // 1100 0110 0000 0000      the Z tetromino with no rotation
#define SHAPE_S_90  ((shape)0x2640) // 0010 0110 0100 0000      the Z tetromino with a 90° rotation
#define SHAPE_S_180 ((shape)0x0C60) // 0000 1100 0110 0000      the Z tetromino with a 180° rotation
#define SHAPE_S_270 ((shape)0x4C80) // 0100 1100 1000 0000      the Z tetromino with a 270° rotation

#define SHAPE_Z     ((shape)0x6C00) // 0110 1100 0000 0000      the S tetromino with no rotation
#define SHAPE_Z_90  ((shape)0x4620) // 0100 0110 0010 0000      the S tetromino with a 90° rotation
#define SHAPE_Z_180 ((shape)0x06C0) // 0000 0110 1100 0000      the S tetromino with a 180° rotation
#define SHAPE_Z_270 ((shape)0x8C40) // 1000 1100 0100 0000      the S tetromino with a 270° rotation

#define SHAPE_T     ((shape)0x0E40) // 0000 1110 0100 0000      the T tetromino with no rotation
#define SHAPE_T_90  ((shape)0x4C40) // 0100 1100 0100 0000      the T tetromino with a 90° rotation
#define SHAPE_T_180 ((shape)0x4E00) // 0100 1110 0000 0000      the T tetromino with a 180° rotation
#define SHAPE_T_270 ((shape)0x4640) // 0100 0110 0100 0000      the T tetromino with a 270° rotation

#define SHAPE_L     ((shape)0x4460) // 0100 0100 0110 0000      the L tetromino with no rotation
#define SHAPE_L_90  ((shape)0x0E80) // 0000 1110 1000 0000      the L tetromino with a 90° rotation
#define SHAPE_L_180 ((shape)0xC440) // 1100 0100 0100 0000      the L tetromino with a 180° rotation
#define SHAPE_L_270 ((shape)0x2E00) // 0010 1110 0000 0000      the L tetromino with a 270° rotation

#define SHAPE_J     ((shape)0x44C0) // 0100 0100 1100 0000      the J tetromino with no rotation
#define SHAPE_J_90  ((shape)0x8E00) // 1000 1110 0000 0000      the J tetromino with a 90° rotation
#define SHAPE_J_180 ((shape)0x6440) // 0110 0100 0100 0000      the J tetromino with a 180° rotation
#define SHAPE_J_270 ((shape)0x0E20) // 0000 1110 0010 0000      the J tetromino with a 270° rotation

shape shape_from_id(shape_id const id) {
    static shape const shapes[TETROMINO_COUNT][4] = {
        // 0°       90°         180°         170°
        {SHAPE_O, SHAPE_O,    SHAPE_O,     SHAPE_O    },
        {SHAPE_I, SHAPE_I_90, SHAPE_I_180, SHAPE_I_270},
        {SHAPE_S, SHAPE_S_90, SHAPE_S_180, SHAPE_S_270},
        {SHAPE_Z, SHAPE_Z_90, SHAPE_Z_180, SHAPE_Z_270},
        {SHAPE_T, SHAPE_T_90, SHAPE_T_180, SHAPE_T_270},
        {SHAPE_L, SHAPE_L_90, SHAPE_L_180, SHAPE_L_270},
        {SHAPE_J, SHAPE_J_90, SHAPE_J_180, SHAPE_J_270},
    };

    // first 3 bits is the shape type, the rest is rotation data
    return shapes[id & 7][id >> 3];
}

colour8 colour_from_id(shape_id const id) {
    switch (id & 7) {
    case TETROMINO_O: return opts.colour_O;
    case TETROMINO_I: return opts.colour_I;
    case TETROMINO_S: return opts.colour_S;
    case TETROMINO_Z: return opts.colour_Z;
    case TETROMINO_T: return opts.colour_T;
    case TETROMINO_L: return opts.colour_L;
    case TETROMINO_J: return opts.colour_J;
    default: return COLOUR8_BLACK;
    }
}
