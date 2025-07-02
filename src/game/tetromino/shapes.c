#include "shapes.h"

#include <string.h>

#include "../../io/colour/colour8.h"
#include "../../util/types.h"
#include "../../util/vec.h"

/*                                   0    1    2    3         */
#define SHAPE_O     ((u16)0x0660) // 0000 0110 0110 0000      the O tetromino with no rotation

#define SHAPE_I     ((u16)0x0F00) // 0000 1111 0000 0000      the I tetromino with no rotation
#define SHAPE_I_90  ((u16)0x2222) // 0010 0010 0010 0010      the I tetromino with a 90° rotation
#define SHAPE_I_180 ((u16)0x00F0) // 0000 0000 1111 0000      the I tetromino with a 180° rotation
#define SHAPE_I_270 ((u16)0x4444) // 0100 0100 0100 0100      the I tetromino with a 270° rotation

#define SHAPE_S     ((u16)0x6C00) // 0110 1100 0000 0000      the S tetromino with no rotation
#define SHAPE_S_90  ((u16)0x4620) // 0100 0110 0010 0000      the S tetromino with a 90° rotation
#define SHAPE_S_180 ((u16)0x06C0) // 0000 0110 1100 0000      the S tetromino with a 180° rotation
#define SHAPE_S_270 ((u16)0x8C40) // 1000 1100 0100 0000      the S tetromino with a 270° rotation

#define SHAPE_Z     ((u16)0xC600) // 1100 0110 0000 0000      the Z tetromino with no rotation
#define SHAPE_Z_90  ((u16)0x2640) // 0010 0110 0100 0000      the Z tetromino with a 90° rotation
#define SHAPE_Z_180 ((u16)0x0C60) // 0000 1100 0110 0000      the Z tetromino with a 180° rotation
#define SHAPE_Z_270 ((u16)0x4C80) // 0100 1100 1000 0000      the Z tetromino with a 270° rotation

#define SHAPE_T     ((u16)0x0E40) // 0000 1110 0100 0000      the T tetromino with no rotation
#define SHAPE_T_90  ((u16)0x4C40) // 0100 1100 0100 0000      the T tetromino with a 90° rotation
#define SHAPE_T_180 ((u16)0x4E00) // 0100 1110 0000 0000      the T tetromino with a 180° rotation
#define SHAPE_T_270 ((u16)0x4640) // 0100 0110 0100 0000      the T tetromino with a 270° rotation

#define SHAPE_L     ((u16)0x4460) // 0100 0100 0110 0000      the L tetromino with no rotation
#define SHAPE_L_90  ((u16)0x0E80) // 0000 1110 1000 0000      the L tetromino with a 90° rotation
#define SHAPE_L_180 ((u16)0xC440) // 1100 0100 0100 0000      the L tetromino with a 180° rotation
#define SHAPE_L_270 ((u16)0x2E00) // 0010 1110 0000 0000      the L tetromino with a 270° rotation

#define SHAPE_J     ((u16)0x44C0) // 0100 0100 1100 0000      the J tetromino with no rotation
#define SHAPE_J_90  ((u16)0x8E00) // 1000 1110 0000 0000      the J tetromino with a 90° rotation
#define SHAPE_J_180 ((u16)0x6440) // 0110 0100 0100 0000      the J tetromino with a 180° rotation
#define SHAPE_J_270 ((u16)0x0E20) // 0000 1110 0010 0000      the J tetromino with a 270° rotation

void shape_getblocks(u8 id, i8vec2* restrict out) {
	struct blockdat {
		i8vec2 memb[4];
	};
	struct blockdat dat;
	switch (id) {
	// O tetromino
	case TETROMINO_O | TETROMINO_ROTATED_0:
	case TETROMINO_O | TETROMINO_ROTATED_90:
	case TETROMINO_O | TETROMINO_ROTATED_180:
	case TETROMINO_O | TETROMINO_ROTATED_270: dat = (struct blockdat){{{1, 1}, {2, 1}, {1, 2}, {2, 2}}}; break;

	// I tetromino
	case TETROMINO_I | TETROMINO_ROTATED_0:   dat = (struct blockdat){{{0, 1}, {1, 1}, {2, 1}, {3, 1}}}; break;
	case TETROMINO_I | TETROMINO_ROTATED_90:  dat = (struct blockdat){{{2, 0}, {2, 1}, {2, 2}, {2, 3}}}; break;
	case TETROMINO_I | TETROMINO_ROTATED_180: dat = (struct blockdat){{{0, 2}, {1, 2}, {2, 2}, {3, 2}}}; break;
	case TETROMINO_I | TETROMINO_ROTATED_270: dat = (struct blockdat){{{1, 0}, {1, 1}, {1, 2}, {1, 3}}}; break;

	// S tetromino
	case TETROMINO_S | TETROMINO_ROTATED_0:   dat = (struct blockdat){{{1, 0}, {2, 0}, {0, 1}, {1, 1}}}; break;
	case TETROMINO_S | TETROMINO_ROTATED_90:  dat = (struct blockdat){{{1, 0}, {1, 1}, {2, 1}, {2, 2}}}; break;
	case TETROMINO_S | TETROMINO_ROTATED_180: dat = (struct blockdat){{{1, 1}, {2, 1}, {0, 2}, {1, 2}}}; break;
	case TETROMINO_S | TETROMINO_ROTATED_270: dat = (struct blockdat){{{0, 0}, {0, 1}, {1, 1}, {1, 2}}}; break;

	// Z tetromino
	case TETROMINO_Z | TETROMINO_ROTATED_0:   dat = (struct blockdat){{{0, 0}, {1, 0}, {1, 1}, {2, 1}}}; break;
	case TETROMINO_Z | TETROMINO_ROTATED_90:  dat = (struct blockdat){{{2, 0}, {1, 1}, {2, 1}, {1, 2}}}; break;
	case TETROMINO_Z | TETROMINO_ROTATED_180: dat = (struct blockdat){{{0, 1}, {1, 1}, {1, 2}, {2, 2}}}; break;
	case TETROMINO_Z | TETROMINO_ROTATED_270: dat = (struct blockdat){{{1, 0}, {0, 1}, {1, 1}, {0, 2}}}; break;

	// T tetromino
	case TETROMINO_T | TETROMINO_ROTATED_0:   dat = (struct blockdat){{{0, 1}, {1, 1}, {2, 1}, {1, 2}}}; break;
	case TETROMINO_T | TETROMINO_ROTATED_90:  dat = (struct blockdat){{{1, 0}, {0, 1}, {1, 1}, {1, 2}}}; break;
	case TETROMINO_T | TETROMINO_ROTATED_180: dat = (struct blockdat){{{1, 0}, {0, 1}, {1, 1}, {2, 1}}}; break;
	case TETROMINO_T | TETROMINO_ROTATED_270: dat = (struct blockdat){{{1, 0}, {1, 1}, {2, 1}, {1, 2}}}; break;

	// L tetromino
	case TETROMINO_L | TETROMINO_ROTATED_0:   dat = (struct blockdat){{{1, 0}, {1, 1}, {1, 2}, {2, 2}}}; break;
	case TETROMINO_L | TETROMINO_ROTATED_90:  dat = (struct blockdat){{{0, 1}, {1, 1}, {2, 1}, {0, 2}}}; break;
	case TETROMINO_L | TETROMINO_ROTATED_180: dat = (struct blockdat){{{0, 0}, {1, 0}, {1, 1}, {1, 2}}}; break;
	case TETROMINO_L | TETROMINO_ROTATED_270: dat = (struct blockdat){{{2, 0}, {0, 1}, {1, 1}, {2, 1}}}; break;

	// J tetromino
	case TETROMINO_J | TETROMINO_ROTATED_0:   dat = (struct blockdat){{{1, 0}, {1, 1}, {0, 2}, {1, 2}}}; break;
	case TETROMINO_J | TETROMINO_ROTATED_90:  dat = (struct blockdat){{{0, 0}, {0, 1}, {1, 1}, {2, 1}}}; break;
	case TETROMINO_J | TETROMINO_ROTATED_180: dat = (struct blockdat){{{1, 0}, {2, 0}, {1, 1}, {1, 2}}}; break;
	case TETROMINO_J | TETROMINO_ROTATED_270: dat = (struct blockdat){{{0, 1}, {1, 1}, {2, 1}, {2, 2}}}; break;
	}

	memcpy(out, dat.memb, sizeof(i8vec2[4]));
}

u16 shape_from_id(u8 id) {
	static u16 const shapes[TETROMINO_COUNT][4] = {
		// 0°       90°         180°         170°
		{SHAPE_O, SHAPE_O, SHAPE_O, SHAPE_O},
		{SHAPE_I, SHAPE_I_90, SHAPE_I_180, SHAPE_I_270},
		{SHAPE_S, SHAPE_S_90, SHAPE_S_180, SHAPE_S_270},
		{SHAPE_Z, SHAPE_Z_90, SHAPE_Z_180, SHAPE_Z_270},
		{SHAPE_T, SHAPE_T_90, SHAPE_T_180, SHAPE_T_270},
		{SHAPE_L, SHAPE_L_90, SHAPE_L_180, SHAPE_L_270},
		{SHAPE_J, SHAPE_J_90, SHAPE_J_180, SHAPE_J_270},
	};

	// first 3 least significant bits is the shape type, the rest is rotation data
	return shapes[id & 7][id >> 3];
}

colour8 colour_from_id(u8 id) {
	switch (id & 7) {
	case TETROMINO_O: return COLOUR8_YELLOW;
	case TETROMINO_I: return COLOUR8_CYAN;
	case TETROMINO_S: return COLOUR8_GREEN;
	case TETROMINO_Z: return COLOUR8_RED;
	case TETROMINO_T: return COLOUR8_MAGENTA;
	case TETROMINO_L: return COLOUR8_ORANGE;
	case TETROMINO_J: return COLOUR8_BLUE;
	default:          return COLOUR8_BLACK;
	}
}
