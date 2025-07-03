#include "shapes.h"

#include "../../io/colour/colour8.h"
#include "../../util/types.h"
#include "../../util/vec.h"
#include "../../error.h"

void shape_getblocks(u8 id, i8vec2* restrict out) {
	struct blockdat {
		u8 ax : 2, ay : 2;
		u8 bx : 2, by : 2;
		u8 cx : 2, cy : 2;
		u8 dx : 2, dy : 2;
	} dat;

	switch (id) {
	// O tetromino
	case TETROMINO_O | TETROMINO_ROTATED_0:
	case TETROMINO_O | TETROMINO_ROTATED_90:
	case TETROMINO_O | TETROMINO_ROTATED_180:
	case TETROMINO_O | TETROMINO_ROTATED_270: dat = (struct blockdat){1, 1, 2, 1, 1, 2, 2, 2}; break;

	// I tetromino
	case TETROMINO_I | TETROMINO_ROTATED_0:   dat = (struct blockdat){0, 1, 1, 1, 2, 1, 3, 1}; break;
	case TETROMINO_I | TETROMINO_ROTATED_90:  dat = (struct blockdat){2, 0, 2, 1, 2, 2, 2, 3}; break;
	case TETROMINO_I | TETROMINO_ROTATED_180: dat = (struct blockdat){0, 2, 1, 2, 2, 2, 3, 2}; break;
	case TETROMINO_I | TETROMINO_ROTATED_270: dat = (struct blockdat){1, 0, 1, 1, 1, 2, 1, 3}; break;

	// S tetromino
	case TETROMINO_S | TETROMINO_ROTATED_0:   dat = (struct blockdat){1, 0, 2, 0, 0, 1, 1, 1}; break;
	case TETROMINO_S | TETROMINO_ROTATED_90:  dat = (struct blockdat){1, 0, 1, 1, 2, 1, 2, 2}; break;
	case TETROMINO_S | TETROMINO_ROTATED_180: dat = (struct blockdat){1, 1, 2, 1, 0, 2, 1, 2}; break;
	case TETROMINO_S | TETROMINO_ROTATED_270: dat = (struct blockdat){0, 0, 0, 1, 1, 1, 1, 2}; break;

	// Z tetromino
	case TETROMINO_Z | TETROMINO_ROTATED_0:   dat = (struct blockdat){0, 0, 1, 0, 1, 1, 2, 1}; break;
	case TETROMINO_Z | TETROMINO_ROTATED_90:  dat = (struct blockdat){2, 0, 1, 1, 2, 1, 1, 2}; break;
	case TETROMINO_Z | TETROMINO_ROTATED_180: dat = (struct blockdat){0, 1, 1, 1, 1, 2, 2, 2}; break;
	case TETROMINO_Z | TETROMINO_ROTATED_270: dat = (struct blockdat){1, 0, 0, 1, 1, 1, 0, 2}; break;

	// T tetromino
	case TETROMINO_T | TETROMINO_ROTATED_0:   dat = (struct blockdat){0, 1, 1, 1, 2, 1, 1, 2}; break;
	case TETROMINO_T | TETROMINO_ROTATED_90:  dat = (struct blockdat){1, 0, 0, 1, 1, 1, 1, 2}; break;
	case TETROMINO_T | TETROMINO_ROTATED_180: dat = (struct blockdat){1, 0, 0, 1, 1, 1, 2, 1}; break;
	case TETROMINO_T | TETROMINO_ROTATED_270: dat = (struct blockdat){1, 0, 1, 1, 2, 1, 1, 2}; break;

	// L tetromino
	case TETROMINO_L | TETROMINO_ROTATED_0:   dat = (struct blockdat){1, 0, 1, 1, 1, 2, 2, 2}; break;
	case TETROMINO_L | TETROMINO_ROTATED_90:  dat = (struct blockdat){0, 1, 1, 1, 2, 1, 0, 2}; break;
	case TETROMINO_L | TETROMINO_ROTATED_180: dat = (struct blockdat){0, 0, 1, 0, 1, 1, 1, 2}; break;
	case TETROMINO_L | TETROMINO_ROTATED_270: dat = (struct blockdat){2, 0, 0, 1, 1, 1, 2, 1}; break;

	// J tetromino
	case TETROMINO_J | TETROMINO_ROTATED_0:   dat = (struct blockdat){1, 0, 1, 1, 0, 2, 1, 2}; break;
	case TETROMINO_J | TETROMINO_ROTATED_90:  dat = (struct blockdat){0, 0, 0, 1, 1, 1, 2, 1}; break;
	case TETROMINO_J | TETROMINO_ROTATED_180: dat = (struct blockdat){1, 0, 2, 0, 1, 1, 1, 2}; break;
	case TETROMINO_J | TETROMINO_ROTATED_270: dat = (struct blockdat){0, 1, 1, 1, 2, 1, 2, 2}; break;

	default:
#ifndef NDEBUG
		fatal(1, "something went wrong; couldn't reconise the ID.", );
#endif
		break;
	}

	out[0] = (i8vec2){dat.ax, dat.ay};
	out[1] = (i8vec2){dat.bx, dat.by};
	out[2] = (i8vec2){dat.cx, dat.cy};
	out[3] = (i8vec2){dat.dx, dat.dy};
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
